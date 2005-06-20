 
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
#include "controler.h"
#include "cutefig.h"
#include "canvasview.h"
#include "allactions.h"
#include "objecthandler.h"
#include "objectmapper.h"

#include <QObject>
#include <QApplication>
#include <QMenu>
#include <QToolButton>
#include <QSignalMapper>

#include <QDebug>

/** This was suggested by the QT4 docs in order to replace the QT3
 * function.
 */
void ActionCollection::addSeparator()
{
        QAction* s = new QAction( this );
        s->setSeparator( true );
        addAction( s );
}

void ActionCollection::setAllEnabled( bool enabled )
{
        foreach ( QAction* a, actions() )
                a->setEnabled( enabled );
}


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
}


/********************** FileActions **************************/

FileActions::FileActions( CuteFig* parent ) 
        : ActionCollection( parent )
{
        setText( tr("&File") );
        QAction* fileNew = new QAction( QIcon(":images/filenew.png"), tr("&New"),  this );
        fileNew->setShortcut( Qt::CTRL+Qt::Key_N );
        connect( fileNew, SIGNAL( triggered() ), parent, SLOT( newDoc() ) );

        QAction* open = new QAction( QIcon(":images/fileopen.png"), tr("&Open"), this );
        open->setShortcut( Qt::CTRL+Qt::Key_O );
        connect( open, SIGNAL( triggered() ), parent, SLOT( choose()) );
        
        QAction* save = new QAction( QIcon(":images/filesave.png"), tr("&Save"), this );
        save->setShortcut( Qt::CTRL+Qt::Key_S );
        connect( save, SIGNAL( triggered() ), parent, SLOT( save() ) );

        QAction* saveAs = new QAction( QIcon(":images/filesaveas.png"), tr("Save &as"), this );
        connect( saveAs, SIGNAL( triggered() ), parent, SLOT( saveAs() ) );

        addSeparator();

        QAction* print = new QAction( QIcon(":images/fileprint.png"), tr("&Print"), this );
        print->setShortcut( Qt::CTRL+Qt::Key_P );
        connect( print, SIGNAL( triggered() ), parent, SLOT( print() ) );

        addSeparator();

        QAction* close = new QAction( QIcon(":images/fileclose.png"), "&Close", this );
        close->setShortcut( Qt::CTRL+Qt::Key_W );
        connect( close, SIGNAL( triggered() ), parent, SLOT( close() ) );

        QAction* quit = new QAction( QIcon(":images/exit.png"), "&Quit", this );
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
        : ActionCollection( parent )
{
        setText( tr("&Edit") );

        undoMenu_ = new QMenu( parent->mainWindow() );
        undo_ = new QAction( QIcon(":images/undo.png"), undoString_, this );
        undo_->setShortcut( Qt::CTRL+Qt::Key_Z );
        connect( undo_, SIGNAL( triggered() ), parent, SLOT( undo() ) );
        undo_->setEnabled( false );

        QToolButton* undoTB = new QToolButton();
        undoTB->setDefaultAction( undo_ );
        undoTB->setMenu( undoMenu_ );
        undoTB->setPopupMode( QToolButton::DelayedPopup );
        specialButtons_[undo_] = undoTB;

        undoSignalMapper_ = new QSignalMapper( this );
        connect( undoSignalMapper_, SIGNAL( mapped(int) ), parent, SLOT( undo(int) ) );

        redoMenu_ = new QMenu( parent->mainWindow() );
        redo_ = new QAction( QIcon(":images/redo.png"), redoString_, this );
        redo_->setShortcut( Qt::CTRL+Qt::Key_R );
        connect( redo_, SIGNAL( triggered() ), parent, SLOT( redo() ) );
        redo_->setEnabled( false );

        redoSignalMapper_ = new QSignalMapper( this );
        connect( redoSignalMapper_, SIGNAL( mapped(int) ), parent, SLOT( redo(int) ) );

        QToolButton* redoTB = new QToolButton();
        redoTB->setDefaultAction( redo_ );
        redoTB->setMenu( redoMenu_ );
        redoTB->setPopupMode( QToolButton::DelayedPopup );
        specialButtons_[redo_] = redoTB;


        addSeparator();

        new CopyAction( parent, this );
        new PasteAction( parent, this );
        new CutAction( parent, this );
        new DeleteAction( parent, this );

        addSeparator();
        new EditdialogAction( parent, this );

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

        const QString menuString = undoString_ + colonSep_ + s;
        QAction* newHead = new QAction( menuString, undoMenu_ );
        undoMenu_->insertAction( undoHead_, newHead );
        undoHead_ = newHead;
        
        undo_->setText( menuString );
        undo_->setEnabled( true );

        remapUndoSignals();
}

/** removes the first item of the undo menu, and adds one to the redo
 *  menu
 */
void EditActions::undo( const QString& s )
{
        undoMenu_->removeAction( undoHead_ );
        delete undoHead_;

        if ( undoMenu_->actions().isEmpty() ) {
                undoHead_ = 0;
                undo_->setText( undoString_ );
                undo_->setEnabled( false );
        } else {
                undoHead_ = undoMenu_->actions().first();
                undo_->setText( undoHead_->text() );
                undo_->setEnabled( true );
        }

        const QString menuString = redoString_ + colonSep_ + s;
        QAction* newHead = new QAction( menuString, redoMenu_ );

        redoMenu_->insertAction( redoHead_, newHead );
        redoHead_ = newHead;

        redo_->setText( menuString );
        redo_->setEnabled( true );

        remapUndoSignals();
        remapRedoSignals();
}

/** removes the first item of the redo menu, and adds one to the undo
 *  menu
 */
void EditActions::redo( const QString& s )
{
        redoMenu_->removeAction( redoHead_ );
        delete redoHead_;

        if ( redoMenu_->actions().isEmpty() ) {
                redoHead_ = 0;
                redo_->setText( redoString_ );
                redo_->setEnabled( false );
        } else {
                redoHead_ = redoMenu_->actions().first();
                redo_->setText( redoHead_->text() );
                redo_->setEnabled( true );
        }

        const QString menuString = undoString_ + colonSep_ + s;
        QAction* newHead = new QAction( menuString, undoMenu_ );

        undoMenu_->insertAction( undoHead_, newHead );
        undoHead_ = newHead;
        undo_->setEnabled( true );

        remapUndoSignals();
        remapRedoSignals();
}


void EditActions::remapUndoSignals()
{
        int i = 0;
        foreach ( QAction* a, undoMenu_->actions() ) {
                undoSignalMapper_->removeMappings( a );
                connect( a, SIGNAL( triggered() ), undoSignalMapper_, SLOT( map() ) );
                undoSignalMapper_->setMapping( a, ++i );
        }
}

void EditActions::remapRedoSignals()
{
        int i = 0;
        foreach ( QAction* a, redoMenu_->actions() ) {
                redoSignalMapper_->removeMappings( a );
                connect( a, SIGNAL( triggered() ), redoSignalMapper_, SLOT( map() ) );
                redoSignalMapper_->setMapping( a, ++i );
         }
}


/************************ ViewActions **************************/

ViewActions::ViewActions( CanvasView* parent )
        : ActionCollection( parent )
{
        setText( tr("&View") );
        
        QAction* zoomIn = new QAction( QIcon(":images/viewmag+.png"), tr("Zoom &in"), this );
        zoomIn->setShortcut( Qt::Key_Plus );
        connect( zoomIn, SIGNAL( triggered() ), parent, SLOT( zoomIn() ) );

        QAction* zoomOut = new QAction( QIcon(":images/viewmag-.png"), tr("Zoom &out"), this );
        zoomOut->setShortcut( Qt::Key_Minus );
        connect( zoomOut, SIGNAL( triggered() ), parent, SLOT( zoomOut() ) );

        QAction* zoom1 = new QAction( QIcon(":images/viewmag1.png"), tr("O&riginal size"), this );
        zoom1->setShortcut( Qt::Key_1 );
        connect( zoom1, SIGNAL( triggered() ), parent, SLOT( zoomOrig() ) );

        QAction* zoomFit = new QAction( QIcon(":images/viewmagfit.png"), tr("Fit canvas"), this );
        zoomFit->setShortcut( Qt::CTRL+Qt::Key_F );
        connect( zoomFit, SIGNAL( triggered() ), parent, SLOT( zoomFit() ) );
}


/************************ CreateActions ************************/

CreateActions::CreateActions( Controler* parent )
        : ActionCollection( parent )
{
        setText( tr("&Add") );
        ObjectMapper& m = ObjectMapper::instance();
        foreach ( ObjectHandler* h, m.objectMap_.values() )
                h->createAction( parent, this );
}


/************************ ToolActions **************************/

ToolActions::ToolActions( Controler* parent )
        : ActionCollection( parent )
{
        setText("&Tools");
        move_  = new MoveAction( parent, this );
        scale_ = new ScaleAction( parent, this );
        new PointMoveAction( parent, this );
        setAllEnabled( true );

        parent->setToolActionsGroup( this );
}


/************************ FormatActions **************************/

FormatActions::FormatActions( Controler* parent )
        : ActionCollection( parent )
{
        setText( tr("&Format") );
        new GroupAction( parent, this );
        new UngroupAction( parent, this );    
}


