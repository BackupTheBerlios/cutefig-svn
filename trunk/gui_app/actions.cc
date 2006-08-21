 
/*****************************************************************************
**
**  @version $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2005 Johannes Mueller, joh@users.berlios.de
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License version 2
**  as published by the Free Software Foundation.
** 
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
******************************************************************************/

#include "actions.h"
#include "allactions.h"
#include "controler.h"
#include "cutefig.h"
#include "canvasview.h"
#include "objectguihandler.h"

#include "zoomcombobox.h"

#include <QObject>
#include <QApplication>
#include <QMenu>
#include <QToolButton>
#include <QSignalMapper>

#include <QDebug>


/********************** AllActions **************************/

AllActions::AllActions( CuteFig* parent )
        : QObject( parent )
{
        Controler* controler = parent->controler();
        
        addActions( new FileActions( parent ) );
        addActions( new EditActions( controler ) );
        addActions( new ViewActions( parent->canvasView() ) );
        addActions( new FormatActions( controler ) );
        addActions( new CreateActions( controler ) );
        addActions( new ToolActions( controler ) );
        addActions( new TextPropActions( controler ) );
}


/********************** FileActions **************************/

FileActions::FileActions( CuteFig* parent ) 
        : ActionCollection( parent )
{
        setText( tr("&File") );
        QAction* fileNew = new QAction( QIcon(":images/filenew.png"), tr("&New"),  this );
        addAction( fileNew );
        fileNew->setShortcut( Qt::CTRL+Qt::Key_N );
        connect( fileNew, SIGNAL( triggered() ), parent, SLOT( newDoc() ) );

        QAction* open = new QAction( QIcon(":images/fileopen.png"), tr("&Open"), this );
        addAction( open );
        open->setShortcut( Qt::CTRL+Qt::Key_O );
        connect( open, SIGNAL( triggered() ), parent, SLOT( choose()) );
        
        QAction* save = new QAction( QIcon(":images/filesave.png"), tr("&Save"), this );
        addAction( save );
        save->setShortcut( Qt::CTRL+Qt::Key_S );
        connect( save, SIGNAL( triggered() ), parent, SLOT( save() ) );

        QAction* saveAs = new QAction( QIcon(":images/filesaveas.png"), tr("Save &as"), this );
        addAction( saveAs );
        connect( saveAs, SIGNAL( triggered() ), parent, SLOT( saveAs() ) );

        addSeparator();

        QAction* exprt = new QAction( tr("E&xport"), this );
        addAction( exprt );
        connect( exprt, SIGNAL( triggered() ), parent, SLOT( exportFigure() ) );
        
        QAction* print = new QAction( QIcon(":images/fileprint.png"), tr("&Print"), this );
        addAction( print );
        print->setShortcut( Qt::CTRL+Qt::Key_P );
        connect( print, SIGNAL( triggered() ), parent, SLOT( print() ) );

        addSeparator();

        QAction* props = new QAction( QIcon(":images/info.png"),tr("Document P&roperties"), this );
        addAction( props );
        connect( props, SIGNAL( triggered() ), parent, SLOT( editProperties() ) );

        addSeparator();

        QAction* close = new QAction( QIcon(":images/fileclose.png"), tr("&Close"), this );
        addAction( close );
        close->setShortcut( Qt::CTRL+Qt::Key_W );
        connect( close, SIGNAL( triggered() ), parent, SLOT( close() ) );

        QAction* quit = new QAction( QIcon(":images/exit.png"), tr("&Quit"), this );
        addAction( quit );
        quit->setShortcut( Qt::CTRL+Qt::Key_Q );
        connect( quit, SIGNAL( triggered() ), qApp, SLOT( closeAllWindows() ) );
}



/************************ EditActions **************************/

const QString EditActions::undoString_ = tr("&Undo");
const QString EditActions::redoString_ = tr("&Redo");
const QString EditActions::colonSep_ = ": ";

/** The constructor has to setup the signal mapper and the undo redo menus 
 *  appart from setting up the actions.
 */
EditActions::EditActions( Controler* parent )
        : ActionCollection( parent ),
          controler_( parent )
{
        setText( tr("&Edit") );
        
        undoMenu_ = new QMenu( parent->mainWindow() );
        undo_ = new QAction( QIcon(":images/undo.png"), undoString_, this );
        addAction( undo_ );
        undo_->setShortcut( Qt::CTRL+Qt::Key_Z );
        connect( undo_, SIGNAL( triggered() ), this, SLOT( undo() ) );
        undo_->setEnabled( false );

        QToolButton* undoTB = new QToolButton();
        undoTB->setDefaultAction( undo_ );
        undoTB->setMenu( undoMenu_ );
        undoTB->setPopupMode( QToolButton::DelayedPopup );
        specialWidgets_[undo_] = undoTB;

        undoSignalMapper_ = new QSignalMapper( this );
        connect( undoSignalMapper_, SIGNAL( mapped(int) ), this, SLOT( undo(int) ) );

        redoMenu_ = new QMenu( parent->mainWindow() );
        redo_ = new QAction( QIcon(":images/redo.png"), redoString_, this );
        addAction( redo_ );
        redo_->setShortcut( Qt::CTRL+Qt::Key_R );
        connect( redo_, SIGNAL( triggered() ), this, SLOT( redo() ) );
        redo_->setEnabled( false );

        redoSignalMapper_ = new QSignalMapper( this );
        connect( redoSignalMapper_, SIGNAL( mapped(int) ), this, SLOT( redo(int) ) );

        QToolButton* redoTB = new QToolButton();
        redoTB->setDefaultAction( redo_ );
        redoTB->setMenu( redoMenu_ );
        redoTB->setPopupMode( QToolButton::DelayedPopup );
        specialWidgets_[redo_] = redoTB;


        addSeparator();

        addAction( new CopyAction( parent ) );
        addAction( new PasteAction( parent ) );
        addAction( new CutAction( parent ) );
        addAction( new DeleteAction( parent ) );

        addSeparator();
        addAction( new EditdialogAction( parent ) );

        parent->setEditActionsGroup( this );
        parent->addActionGroup( this );
}

/** Clears the redo menu, modifys the text of undo_ and adds an item
 * to the undo menu.
 */
void EditActions::addCommand( const QString& s )
{
        redoMenu_->clear();
        redo_->setEnabled( false );
        redo_->setText( redoString_ );

        undoMenu_->insertAction( undoMenu_->actions().value( 0 ), new QAction( s, undoMenu_ ) );
        remapUndoSignals();
}

void EditActions::undo( int steps )
{
        const QList<QAction*> actions = undoMenu_->actions();

        for ( int i = 0; i < steps; ++i ) {
                QAction* a = actions[i];
                undoMenu_->removeAction( a );
                redoMenu_->insertAction( redoMenu_->actions().value( 0 ), a );
        }

        remapUndoSignals();
        remapRedoSignals();

        controler_->undo( steps );
}

void EditActions::redo( int steps )
{
        const QList<QAction*> actions = redoMenu_->actions();

        for ( int i = 0; i < steps; ++i ) {
                QAction* a = actions[i];
                redoMenu_->removeAction( a );
                undoMenu_->insertAction( undoMenu_->actions().value( 0 ), a );
        }

        remapUndoSignals();
        remapRedoSignals();

        controler_->redo( steps );
}


void EditActions::remapUndoSignals()
{
        if ( undoMenu_->actions().isEmpty() ) {
                undo_->setEnabled( false );
                undo_->setText( undoString_ );
                return;
        } 

        undo_->setEnabled( true );
        undo_->setText( undoString_ + colonSep_ + undoMenu_->actions().first()->text() );
        
        int i = 0;
        foreach ( QAction* a, undoMenu_->actions() ) {
                undoSignalMapper_->removeMappings( a );
                a->disconnect();
                connect( a, SIGNAL( triggered() ), undoSignalMapper_, SLOT( map() ) );
                undoSignalMapper_->setMapping( a, ++i );
        }
}

void EditActions::remapRedoSignals()
{
        if ( redoMenu_->actions().isEmpty() ) {
                redo_->setEnabled( false );
                redo_->setText( redoString_ );
                return;
        }

        redo_->setEnabled( true );
        redo_->setText( redoString_ + colonSep_ + redoMenu_->actions().first()->text() );
        
        int i = 0;
        foreach ( QAction* a, redoMenu_->actions() ) {
                redoSignalMapper_->removeMappings( a );
                a->disconnect();
                connect( a, SIGNAL( triggered() ), redoSignalMapper_, SLOT( map() ) );
                redoSignalMapper_->setMapping( a, ++i );
        }
}


/************************ ViewActions **************************/

ViewActions::ViewActions( CanvasView* parent )
        : ActionCollection( parent ),
          cview_( parent )
{
        setText( tr("&View") );
        
        QAction* zoomIn = new QAction( QIcon(":images/viewmag+.png"), tr("Zoom &in"), this );
        zoomIn->setShortcut( Qt::Key_Plus );
        connect( zoomIn, SIGNAL( triggered() ), parent, SLOT( zoomIn() ) );
        addAction( zoomIn );

        QAction* zoomOut = new QAction( QIcon(":images/viewmag-.png"), tr("Zoom &out"), this );
        zoomOut->setShortcut( Qt::Key_Minus );
        connect( zoomOut, SIGNAL( triggered() ), parent, SLOT( zoomOut() ) );
        addAction( zoomOut );
        
        QAction* zoom1 = new QAction( QIcon(":images/viewmag1.png"), tr("O&riginal size"), this );
        zoom1->setShortcut( Qt::Key_1 );
        connect( zoom1, SIGNAL( triggered() ), parent, SLOT( zoomOrig() ) );
        addAction( zoom1 );

        QAction* zoomFit = new QAction( QIcon(":images/viewmagfit.png"), tr("Fit canvas"), this );
        zoomFit->setShortcut( Qt::CTRL+Qt::Key_F );
        connect( zoomFit, SIGNAL( triggered() ), parent, SLOT( zoomFit() ) );
        addAction( zoomFit );
        
        QAction* setZoom = new QAction( tr("&Set zoom"), this );
        setupZoomMenu();
        setZoom->setMenu( zoomMenu_ );
        specialWidgets_[setZoom] = zoomComboBox_;
        addAction( setZoom );

        QAction* gridDialog = new QAction( tr("Snap / &Grid"), this );
        gridDialog->setShortcut( Qt::CTRL+Qt::Key_G );
        connect( gridDialog, SIGNAL( triggered() ), parent, SLOT( newSnapGrid() ) );
        addAction( gridDialog );

        QAction* refineGrid = new QAction( tr("Re&fine grid"), this );
        refineGrid->setShortcut( Qt::CTRL+Qt::Key_Minus );
        connect( refineGrid, SIGNAL( triggered() ), parent, SLOT( refineGrid() ) );
        addAction( refineGrid );
        
        QAction* corsenGrid = new QAction( tr("&Corsen grid" ), this );
        corsenGrid->setShortcut( Qt::CTRL+Qt::Key_Plus );
        connect( corsenGrid, SIGNAL( triggered() ), parent, SLOT( corsenGrid() ) );
        addAction( corsenGrid );
}

void ViewActions::setupZoomMenu()
{
        zoomComboBox_ = new ZoomComboBox();
        zoomMenu_ = new QMenu( cview_->mainWindow() );

        zoomLevels_ << 0.25 << 0.33 << 0.5 << 0.67 << 0.75 << 1.0 << 1.5 << 2.0 << 3.0 << 4.0;

        int i = 0;
        zoomSignalMapper_ = new QSignalMapper( this );
        foreach ( double z, zoomLevels_ ) {
                QString zs = QString::number( z*100 ) + " %";
                zoomComboBox_->addItem( zs, QVariant( z ) );
                QAction* za = new QAction( zs, zoomMenu_ );
                za->setCheckable( true );
                connect( za, SIGNAL( triggered() ), zoomSignalMapper_, SLOT( map() ) );
                zoomMenu_->addAction( za );
                zoomSignalMapper_->setMapping( za, i++ );
        }

        connect( zoomSignalMapper_, SIGNAL( mapped( int ) ), this, SLOT( zoomChanged( int ) ) );
        connect( zoomComboBox_, SIGNAL( zoomChanged( double ) ), this, SLOT( setZoom(double) ) );
        connect( cview_, SIGNAL( scaleChanged(double) ), this, SLOT( setZoom(double) ) );        
}

void ViewActions::zoomChanged( int id )
{
        const double& zoom = zoomLevels_[id];
        setZoom( zoom );
}

void ViewActions::setZoom( double zoom )
{
        if ( sender() != cview_ ) {
                cview_->blockSignals( true );
                cview_->setZoom( zoom );
                cview_->blockSignals( false );
        }

        if ( sender() != zoomComboBox_ ) 
                zoomComboBox_->changeZoom( zoom );
        
        for ( int i = 0; i < zoomLevels_.size(); i++ ) {
                QAction* a = (QAction *) zoomSignalMapper_->mapping( i );
                a->setChecked( zoomLevels_[i] == zoom );
        }
}

/************************ CreateActions ************************/

CreateActions::CreateActions( Controler* parent )
        : ActionCollection( parent )
{
        setText( tr("&Add") );
        ObjectGUIHandler::setupCreateActions( parent, this );
}


/************************ ToolActions **************************/

ToolActions::ToolActions( Controler* parent )
        : ActionCollection( parent )
{
        setText("&Tools");
        move_ = new MoveAction( parent );
        addAction( move_ );
        scale_ = new ScaleAction( parent );
        addAction( scale_ );
        addAction( new PointMoveAction( parent ) );
        setAllEnabled( true );

        parent->setToolActionsGroup( this );
}


/************************ FormatActions **************************/

FormatActions::FormatActions( Controler* parent )
        : ActionCollection( parent )
{
        setText( tr("&Format") );
        addAction( new GroupAction( parent ) );
        addAction( new UngroupAction( parent ) );    
}


/************************ TextPropActions **************************/

TextPropActions::TextPropActions( Controler* parent )
        : ActionCollection( parent )
{
        setText( tr("Text &properties") );
        addAction( new TextHCenterAction( parent ) );
        addAction( new TextLeftAction( parent ) );
        addAction( new TextRightAction( parent ) );
        addAction( new TextTopAction( parent ) );
        addAction( new TextVCenterAction( parent ) );
        addAction( new TextBottomAction( parent ) );

        addAction( new TextBoldAction( parent ) );
        addAction( new TextItalicAction( parent ) );
        addAction( new TextSuperScriptAction( parent ) );
        addAction( new TextSubScriptAction( parent ) );

        parent->setTextPropActions( this );
}

