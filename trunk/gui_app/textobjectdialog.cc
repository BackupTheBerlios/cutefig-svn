 
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

#include "textobjectdialog.h"
#include "textobject.h"
#include "fontbutton.h"
#include "strokewidget.h"
#include "editdialogaction.h"
#include "penwidget.h"

#include <QLayout>
#include <QTextEdit>
#include <QLabel>
#include <QSpinBox>

TextObjectDialog::TextObjectDialog( DrawObject* o, EditdialogAction* a, QWidget* parent )
        : ObjectDialog( o,a, parent )
{
        setWindowTitle( tr("Text Properties") );
        QWidget* page = new QWidget();
        tabWidget->addTab( page, "&Text" );
        
        QHBoxLayout* mainLayout = new QHBoxLayout( page );
        
        QVBoxLayout* lcl = new QVBoxLayout();

        mainLayout->addLayout( lcl );

        QVBoxLayout* rcl = new QVBoxLayout();
        
        fontButton_ = new FontButton( tr("Choose &Font"), page );
        rcl->addWidget( fontButton_ );

        mainLayout->addLayout( rcl );

        penWidget->setEnabled( false );
        fillStroke->setEnabled( false );
}

void TextObjectDialog::setUpConnectionsPrivate()
{
        connect( fontButton_, SIGNAL( fontChanged(const QFont&) ),
                 textObject_, SLOT( setFont(const QFont& ) ) );
        connect( fontButton_, SIGNAL( fontChanged(const QFont&) ),
                 action_, SLOT( wObjectHasChanged() ) );
}

void TextObjectDialog::setDefaultValuesPrivate()
{
        fontButton_->setFont( textObject_->font() );
}

void TextObjectDialog::castDrawObject()
{
        textObject_ = qobject_cast<TextObject*>( drawObject_ );
}
