 
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

#include "exportgui.h"
#include "filterlib.h"
#include "exportfilter.h"
#include "filterfactory.h"
#include "exportdialog.h"
#include "pixoutdialog.h"

#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QByteArray>
#include <QMessageBox>
#include <QApplication>

#include <cstdlib>
#include <cerrno>

#include <QDebug>

ExportGUI::ExportGUI()
        : QObject( qApp )
{
        
}

ExportGUI& ExportGUI::instance()
{
        static ExportGUI* inst = new ExportGUI();
        return *inst;
}

void ExportGUI::exportFigure( Figure* figure )
{
        FilterLib flib = FilterLib::instance();
        QString fFilter = flib.filterString();

        FilterFactory* filterFactory = 0;
        QString filename;
        
        while ( !filterFactory ) {
                filename = QFileDialog::getSaveFileName( 0, tr("File to export to:"),
                                                         QString(), fFilter );

                if ( filename.isEmpty() )
                        return;

                QByteArray format = filename.section( '.', -1 ).toAscii();
                filterFactory = flib.getFilterFactory( format );
                if ( !filterFactory )
                        QMessageBox::warning( 0, tr("Unkown format"),
                                              tr("The figure can not be exported to the format"
                                                 " %1.\nThe format is not known.")
                                              .arg( QString(format) ),
                                              QMessageBox::Ok, QMessageBox::NoButton );
        }        
        
        ExportFilter* filter = filterFactory->filter();
        filter->setFigure( figure );

        ExportDialog* dialog = filterFactory->dialog( filter );

        if ( !dialog->exec() )
                return;

        QFile file( filename );
        if ( !file.open( QIODevice::WriteOnly ) ) {
                QMessageBox::warning( 0, tr("Export failed"),
                                      tr("The figure could not be exportet to %1.\n%2")
                                      .arg( filename ).arg( strerror( errno ) ),
                                      QMessageBox::Ok, QMessageBox::NoButton );
                return;
        }

        filter->setFile( &file );
        filter->exportFigure();
}
