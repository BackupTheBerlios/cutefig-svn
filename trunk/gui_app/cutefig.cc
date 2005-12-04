 
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

#include "fig.h"

#include "cutefig.h"

#include "cfigoutput.h"
#include "centralwidget.h"
#include "canvasview.h"
#include "controler.h"
#include "figure.h"
#include "parser.h"
#include "guiinit.h"

#include "actions.h"
#include "errorreporter.h"

#include "exportgui.h"

#include <QtGui>

#include <fstream>

int CuteFig::rulerWidth = 30;
double CuteFig::unit = Fig::cm2pix;

/** The constructor of CuteFig. It constructs all the three items
 *  which are needed for the MVC design pattern. Then it triggers the
 *  initialisation by calling init() via the delayed initialisation
 *  idiom described in http://www.codeskipper.org/.
 */
CuteFig::CuteFig()
{
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setAttribute( Qt::WA_DeleteOnClose );
        setWindowTitle( tr("The CuteFig drawing system") );

        
        figure_ = new Figure( this );
        controler_ = new Controler( this );
        controler_->setFigure( figure_ );

        cview_ = new CanvasView( controler_, figure_, this );
        
        controler_->addView( cview_ );
        
        CentralWidget* cw = new CentralWidget( cview_, this );
        setCentralWidget( cw );

        viewport_ = cw->viewport();

        QTimer::singleShot( 0, this, SLOT( init() ) );
}

void CuteFig::init()
{
        GUIInitialiser::go();
        setupActions();
        
        int argc = qApp->argc();
        char** argv = qApp->argv();
        
        if ( argc > 1) {
                filename_ = argv[1];
                load( filename_ );
        }
        statusBar()->showMessage("Hello");    
}

void CuteFig::newDoc()
{
        controler_->resetFigure();
}

void CuteFig::choose()
{
        QString fn = QFileDialog::getOpenFileName( this, 
                                                   tr("Choose a file to open"),
                                                   QString(),
                                                   "*cig" );
        if ( !fn.isEmpty() )
                load( fn );
        else
               statusBar()->showMessage( tr("Loading aborted"), 2000 );
}

/** Loads the file filename. Then it parses its contents into
 * the figure_. If any errors occour a report is shown. Finally cview_
 * gets an update message so that the figure is displayed.
 */
void CuteFig::load( const QString& fileName )
{
        QFile f( fileName );
        if ( !f.open( QIODevice::ReadOnly ) )
                return;

        filename_ = fileName;
        
        controler_->resetFigure();
        
        QTextStream ts( &f );    
        Parser p( &ts, figure_, this );
        QString errors = p.parse();

        if ( !errors.isEmpty() ) 
                ErrorReporter::report( errors );

        cview_->updateFigure();
}

/** Uses an OutputBackend to store the figure into a file. The
 * OutputBackand is chosen depending on the filename extension.
 */
void CuteFig::save()
{
        if ( filename_.isEmpty() ) {
                saveAs();
                return;
        }


        std::ofstream ts( filename_.toLocal8Bit().constData() );
        if ( !ts ) {
                statusBar()->showMessage( QString(tr("Could not write to %1"))
                                          .arg(filename_), 2000 );
                return;
        }

        uint slen = filename_.length() - filename_.indexOf('.');
        QString suffix = filename_.right( slen );

        OutputBackend* b;
//         if ( suffix == ".fig" )
//                 b = new XFigOutput( ts );
//         else
        b = new CfigOutput( ts, *figure_ );
                
        b->processOutput();

        delete b;

        ts.close();

        setWindowTitle( tr( "CuteFig: %1" ).arg( filename_ ) );

        controler_->figureSaved();
        statusBar()->showMessage( tr("File %1 saved").arg( filename_ ), 2000 );
}

void CuteFig::saveAs()
{
        QString fn = QFileDialog::getSaveFileName( this,
                                                   tr("Choose filename"),
                                                   QString(),
                                                   "*.cig" );
        if ( !fn.isEmpty() ) {
                filename_ = fn;
                save();
        } else {
                statusBar()->showMessage( tr("Saving aborted"), 2000 );
        }
}

void CuteFig::print()
{
        QMessageBox::information( this, "Does not work", "No Printing yet" );
}

/** Asks the obligated areyousure-question before accepting the close
 * event. This might sometimes be skiped in development versions. :-)
 */
void CuteFig::closeEvent( QCloseEvent* ce )
{
        ce->accept();
        return;
        if ( !controler_->figureChanged() ) {
                ce->accept();
                return;
        }
        
        switch( QMessageBox::information( this, "CuteFig",
                                          tr("The document has been changed "
                                          "since the last save."),
                                          tr("Save Now"), 
                                          tr("Cancel"), tr("Leave Anyway"),
                                          0, 1 ) ) {
            case 0:
                    save();
                    ce->accept();
                    break;
            case 1:
            default: // just for sanity
                    ce->ignore();
                    break;
            case 2:
                    ce->accept();
                    break;
        }
}


void CuteFig::about()
{
        QString s = tr("A vector based drawing programme\n" 
                       "CopyLeft by Johannes Mueller "
                       "<joh@users.berlios.de>");
        
        QMessageBox::about( this, "CuteFig", s );
}

/** Sets up an AllActions instance and then sets up a menubar entry
 * and a toolbutton (if possible) for each ActionCollection. Finally
 * it adds the help menu by foot.
 */
void CuteFig::setupActions()
{
        AllActions* actions = new AllActions( this );

        foreach ( ActionCollection* ac, actions->actionGroups() ) {

                QMenu* menu = new QMenu( ac->text(), this );
                QToolBar* toolBar = new QToolBar( ac->text(), this );

                foreach ( QAction* a, ac->actions() ) {
                        menu->addAction( a );
                        QWidget* tbw = ac->toolBarWidget( a );
                        if ( tbw ) {
                                tbw->setParent( toolBar );
                                toolBar->addWidget( tbw );
                        } else if ( !a->icon().isNull() ) 
                                toolBar->addAction( a );
                }

                if ( toolBar->actions().isEmpty() )
                        delete toolBar;
                else 
                        addToolBar( toolBar );

                if ( menu->actions().isEmpty() )
                        delete menu;
                else
                        menuBar()->addMenu( menu );
        }

        menuBar()->addSeparator();
        
        QMenu * helpMenu = new QMenu( tr("&Help"), this );
        menuBar()->addMenu( helpMenu );
        helpMenu->addAction( "&About", this, SLOT(about()), Qt::Key_F1 );
        helpMenu->addSeparator();
//        helpMenu->addAction( QWhatsThis::createAction( this ) );
}

void CuteFig::exportFigure()
{
        ExportGUI::instance().exportFigure( figure_ );
}

