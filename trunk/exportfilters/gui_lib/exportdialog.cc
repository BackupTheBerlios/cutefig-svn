 
/*****************************************************************************
**
**  @version $Id: pixoutput.h 8 2005-06-21 06:30:27Z joh $
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

#include "exportdialog.h"

#include <QPushButton>
#include <QLayout>

ExportDialog::ExportDialog( QWidget* parent )
        : QDialog( parent )
{
        QVBoxLayout* topLayout = new QVBoxLayout( this );

        mainWidget_ = new QWidget( this );
        topLayout->addWidget( mainWidget_ );

        QHBoxLayout* bottomLayout = new QHBoxLayout( topLayout->widget() );
        topLayout->addItem( bottomLayout );

        QPushButton* ok = new QPushButton( tr("Ok"), this );
        ok->setAutoDefault( true );
        connect( ok, SIGNAL( clicked() ), this, SLOT( accept() ) );
        bottomLayout->addWidget( ok );

        bottomLayout->addSpacing( 10 );
        
        QPushButton* cancel = new QPushButton( tr("Cancel"), this );
        connect( cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
        bottomLayout->addWidget( cancel );
}

