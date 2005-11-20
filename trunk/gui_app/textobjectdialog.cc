 
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

#include <QLayout>
#include <QTextEdit>
#include <QLabel>
#include <QSpinBox>

TextObjectDialog::TextObjectDialog( DrawObject* o, EditdialogAction* a, QWidget* parent )
        : ObjectDialog( o,a, parent )
{
        setWindowTitle( tr("Text Properties") );
        castDrawObject();

        QHBoxLayout* mainLayout = new QHBoxLayout();

        QVBoxLayout* lcl = new QVBoxLayout();
        
        stroke_ = new StrokeWidget( tr("Text &stroke"), this );
        lcl->addWidget( stroke_ );

        QHBoxLayout* dlt = new QHBoxLayout();
        depth = new QSpinBox( this );
        depth->setMinimum(0);
        depth->setMaximum(999);
        depth->setSingleStep(1);
        QLabel* dlb = new QLabel( tr("&Depth"), this );
        dlb->setBuddy( depth );
        dlt->addWidget( dlb );
        dlt->addWidget( depth );

        lcl->addLayout( dlt );

        mainLayout->addLayout( lcl );

        QVBoxLayout* rcl = new QVBoxLayout();
        
        fontButton_ = new FontButton( tr("Choose &Font"), this );
        rcl->addWidget( fontButton_ );

        rcl->addStretch();
        
        comment = new QTextEdit( this );
//        comment->setTextFormat( Qt::PlainText );
        comment->setTabChangesFocus( true );
        QLabel* clb = new QLabel( tr("&Comment"), this );
        clb->setBuddy( comment );
        rcl->addWidget( clb );
        rcl->addWidget( comment );

        mainLayout->addLayout( rcl );
        
        dialogLayout_->insertLayout( 0, mainLayout );
        
        setUpConnections();
        setDefaultValues();
}

void TextObjectDialog::setUpConnections()
{
        connect( depth, SIGNAL( valueChanged(int) ), drawObject_, SLOT( setDepth(int) ) );
        connect( depth, SIGNAL( valueChanged(int) ), action_, SLOT( wObjectHasChanged() ) );
        
        connect( stroke_, SIGNAL( strokeChanged() ), action_, SLOT( wObjectHasChanged() ) );
        
        connect( fontButton_, SIGNAL( fontChanged(const QFont&) ),
                 textObject_, SLOT( setFont(const QFont& ) ) );
        connect( fontButton_, SIGNAL( fontChanged(const QFont&) ),
                 action_, SLOT( wObjectHasChanged() ) );
}

void TextObjectDialog::setDefaultValues()
{
        depth->setValue( drawObject_->depth() );
        stroke_->setStroke( drawObject_->p_stroke() );
        fontButton_->setFont( textObject_->font() );
}

void TextObjectDialog::castDrawObject()
{
        textObject_ = qobject_cast<TextObject*>( drawObject_ );
}
