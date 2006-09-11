 
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

#include "recentfiles.h"
#include "cfigoutput.h"
#include "centralwidget.h"
#include "canvasview.h"
#include "controler.h"
#include "figure.h"
#include "parser.h"
#include "guiinit.h"
#include "actions.h"
#include "errorreporter.h"
#include "propdialog.h"
#include "statuswidgets.h"
#include "exportgui.h"

#include <QtGui>
//#include <QAssistantClient>

#include <fstream>

int CuteFig::rulerWidth = 30;
double CuteFig::unit = Fig::cm2pix;

/*! The constructor of CuteFig. It constructs all the three items
 *  which are needed for the MVC design pattern. Then it triggers the
 *  initialisation by calling init() via the delayed initialisation
 *  idiom described in http://www.codeskipper.org/.
 */
CuteFig::CuteFig()
{
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setAttribute( Qt::WA_DeleteOnClose );
        setWindowTitle( tr("The CuteFig drawing system") );

        Initialiser::initResLibs();
        
        figure_ = new Figure( this );
        controler_ = new Controler( this );
        controler_->setFigure( figure_ );

        ActionStatusIndicator* asi = new ActionStatusIndicator( statusBar() );

        cview_ = new CanvasView( controler_, asi, figure_, this );
        
        controler_->addView( cview_ );
        
        CentralWidget* cw = new CentralWidget( cview_, this );
        setCentralWidget( cw );

        viewport_ = cw->viewport();

        CoordWidget* crd = new CoordWidget( *figure_ );
        connect( cview_, SIGNAL(cursorMovedTo(const QPoint&)),
                 crd, SLOT(setCoords(const QPoint&)) );
        connect( cview_, SIGNAL(cursorIsIn(bool)), crd, SLOT(setIndicating(bool)) );
        
        statusBar()->addPermanentWidget( crd );
        
        QTimer::singleShot( 0, this, SLOT( init() ) );
}

void CuteFig::init()
{
        setupActions();
        
        int argc = qApp->argc();
        char** argv = qApp->argv();
        
        if ( argc > 1) {
                filename_ = QFileInfo( argv[1] ).absoluteFilePath();
                load( filename_ );
        }

        controler_->selection().updateActions();
        readSettings();
        
        statusBar()->showMessage("Hello");
        cview_->setZoom( 1.0 );
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

/*! Loads the file filename. Then it parses its contents into
 * the figure_. If any errors occour a report is shown. Finally cview_
 * gets an update message so that the figure is displayed.
 */
void CuteFig::load( const QString& fileName )
{
        QFile f( fileName );
        if ( !f.open( QIODevice::ReadOnly ) )
                return;

        filename_ = fileName;
        RecentFiles::addFile( fileName );
        
        controler_->resetFigure();
        
        QTextStream ts( &f );
        QString errors = Parser::parse( ts, figure_ );

        if ( !errors.isEmpty() ) 
                ErrorReporter::report( errors );

        cview_->updateFigure();
}

/*! Uses an OutputBackend to store the figure into a file. The
 *  OutputBackand is chosen depending on the filename extension.
 */
void CuteFig::save()
{
        if ( filename_.isEmpty() ) {
                saveAs();
                return;
        }

        figure_->updateModificationDate();
        
        std::ofstream ts( filename_.toLocal8Bit().constData() );
        if ( !ts ) {
                statusBar()->showMessage( QString(tr("Could not write to %1"))
                                          .arg(filename_), 2000 );
                return;
        }

        uint slen = filename_.length() - filename_.indexOf('.');
        QString suffix = filename_.right( slen );

        OutputBackend* b;
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
                RecentFiles::addFile( fn );
                save();
        } else {
                statusBar()->showMessage( tr("Saving aborted"), 2000 );
        }
}

void CuteFig::print()
{
        QPrinter printer;
        QPrintDialog pdlg( &printer, this );

        if ( pdlg.exec() == QDialog::Accepted ) {
                QPainter p( &printer );
                figure_->drawElements( &p );
        }
}


/*! Asks the obligated areyousure-question before accepting the close
 * event. This might sometimes be skiped in development versions. :-)
 */
void CuteFig::closeEvent( QCloseEvent* ce )
{
        if ( !controler_->figureChanged() ) 
                goto finish;
        
        switch( QMessageBox::information( this, "CuteFig",
                                          tr("The document has been changed "
                                             "since the last save."),
                                          tr("Save Now"), 
                                          tr("Cancel"), tr("Leave Anyway"),
                                          0, 1 ) ) {
            case 0:
                    save();
                    break;
            case 1:
            default: // just for sanity
                    ce->ignore();
                    return;
            case 2:
                    break;
        }

 finish:
        writeSettings();
        ResourceIOFactory::saveResLibs();
        ce->accept();
}


void CuteFig::about()
{
        QString s = tr("A vector based drawing programme\n" 
                       "CopyLeft by Johannes Mueller "
                       "<joh@users.berlios.de>");
        
        QMessageBox::about( this, "CuteFig", s );
}

void CuteFig::help()
{
//         QAssistantClient* assi = new QAssistantClient( QString(), this );
//         QSettings s;
        
//         QString helpFile = s.value("HandBookPath", QDir::currentPath() ).toString() +
//                            "/doc/html/" +
//                            QLocale::system().name().left( 2 ) +
//                            "/index.html";
//         assi->showPage( helpFile );
}

/*! Sets up an AllActions instance and then sets up a menubar entry
 * and a toolbutton (if possible) for each ActionCollection. Finally
 * it adds the help menu by foot.
 */
void CuteFig::setupActions()
{
        AllActions* actions = new AllActions( this );

        foreach ( ActionCollection* ac, actions->actionGroups() ) {

                QMenu* menu = new QMenu( ac->text(), this );
                QToolBar* toolBar = new QToolBar( ac->text(), this );

                bool lastWasSep = true;
                foreach ( QAction* a, ac->actions() ) {
                        menu->addAction( a );
                        QWidget* tbw = ac->toolBarWidget( a );
                        if ( tbw ) {
                                tbw->setParent( toolBar );
                                toolBar->addWidget( tbw );
                                lastWasSep = false;
                        } else if ( !a->icon().isNull() ) { 
                                toolBar->addAction( a );
                                lastWasSep = false;
                        } else if ( a->isSeparator() && !lastWasSep ) {
                                toolBar->addSeparator();
                                lastWasSep = true;
                        }
                }

                if ( toolBar->actions().isEmpty() )
                        delete toolBar;
                else {
                        toolBar->setObjectName( ac->metaObject()->className() );
                        addToolBar( toolBar );
                }

                if ( menu->actions().isEmpty() )
                        delete menu;
                else
                        menuBar()->addMenu( menu );
        }

        menuBar()->addSeparator();
        
        QMenu * helpMenu = new QMenu( tr("&Help"), this );
        menuBar()->addMenu( helpMenu );
        helpMenu->addAction( tr("CuteFig &Handbook"), this, SLOT(help()), Qt::Key_F1 );
        helpMenu->addAction( tr("&About"), this, SLOT(about()) );

        
        helpMenu->addSeparator();

//        helpMenu->addAction( QWhatsThis::createAction( this ) );
}

void CuteFig::exportFigure()
{
        ExportGUI::instance().exportFigure( figure_ );
}


void CuteFig::readSettings()
{
        QSettings s;

        s.beginGroup("MainWindow");
        resize( s.value("size", size() ).toSize() );
        restoreState( s.value("state").toByteArray() );
        s.endGroup();
}

void CuteFig::writeSettings()
{
        QSettings s;

        s.beginGroup("MainWindow");
        s.setValue("size", size() );
        s.setValue("state", saveState() );
        s.endGroup();   
}

