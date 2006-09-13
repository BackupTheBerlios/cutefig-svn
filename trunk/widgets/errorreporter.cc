 
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

#include "errorreporter.h"

#include <QGridLayout>
#include <QTextEdit>
#include <QString>
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>

ErrorReporter::ErrorReporter( const QString& text, QWidget * parent )
        : QDialog( parent, false )
{
        setWindowTitle( tr("Error parsing file") );
        
        QGridLayout* l = new QGridLayout;
        l->setSpacing( 5 );
        
        QLabel* icon = new QLabel;
        icon->setPixmap( QMessageBox::standardIcon(QMessageBox::Warning) );

        QLabel* message = new QLabel( tr("The following errors occured...") );

        QTextEdit* contents = new QTextEdit;
        contents->setReadOnly( true );
        contents->setPlainText( text );
        contents->setMinimumSize( 500,200 );

        QPushButton* ok = new QPushButton( tr("Ok"), this );
        ok->setDefault( true );
        connect( ok, SIGNAL( clicked() ), this, SLOT( accept() ) );

        l->addWidget( icon, 1,0, Qt::AlignTop );
        l->addWidget( message, 0,0, 1,3 );
        l->addWidget( contents, 1,1, 1,3 );
        l->addWidget( ok, 2,2 );

	setLayout( l );
}

void ErrorReporter::report( const QString& text, QWidget* parent )
{
        ErrorReporter r( text,parent );
        r.exec();
}
