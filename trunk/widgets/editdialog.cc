 
/*****************************************************************************
**
**  $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2005 Johannes Mueller, johmue@users.sourceforge.net
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

#include "editdialog.h"

#include <QLayout>
#include <QPushButton>

EditDialog::EditDialog( QWidget* parent )
        : QDialog( parent ),
          dialogLayout_( new QVBoxLayout( this ) )
{
        QHBoxLayout* bottomLayout = new QHBoxLayout();

        QPushButton* ok = new QPushButton( tr("&Ok"), this );
        connect( ok, SIGNAL( clicked() ), this, SLOT( accept() ) );
        ok->setAutoDefault( true );

        QPushButton* cancel = new QPushButton( tr("Cancel"), this );
        connect( cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

        QPushButton* reset = new QPushButton( tr("&Reset"), this );
        connect( reset, SIGNAL( clicked() ), this, SLOT( reset() ) );
        
        bottomLayout->addWidget( reset );
        bottomLayout->addStretch();
        bottomLayout->addWidget( ok );
        bottomLayout->addWidget( cancel );

        dialogLayout_->insertLayout( 1, bottomLayout );
}
