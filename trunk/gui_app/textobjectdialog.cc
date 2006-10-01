 
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
#include "fontwidget.h"
#include "strokewidget.h"
#include "editdialogaction.h"
#include "penwidget.h"

#include <QLayout>
#include <QTextEdit>
#include <QSpinBox>

#include <QDebug>

TextObjectDialog::TextObjectDialog( DrawObject* o, EditdialogAction* a, QWidget* parent )
        : ObjectDialog( o,a, parent )
{
        setWindowTitle( tr("Text Properties") );
        QWidget* page = new QWidget();
        tabWidget()->addTab( page, "&Text" );
        
        QHBoxLayout* mainLayout = new QHBoxLayout( page );
        
        QVBoxLayout* lcl = new QVBoxLayout();

        mainLayout->addLayout( lcl );

        QVBoxLayout* rcl = new QVBoxLayout();
        
        fontWidget_ = new FontWidget;
        rcl->addWidget( fontWidget_ );

        mainLayout->addLayout( rcl );

        penWidget()->setEnabled( false );
//         fillStroke->setEnabled( false );
}

void TextObjectDialog::commitChangesPrivate()
{
	qDebug() << "TextObjectDialog::commitChangesPrivate()"
		 << fontWidget_->font().family()
		 << fontWidget_->font().pointSize();
        textObject_->setFont( fontWidget_->font() );
}

void TextObjectDialog::setUpConnectionsPrivate()
{
	qDebug() << "TextObjectDialog::setUpConnectionsPrivate()";
        connect( fontWidget_, SIGNAL( fontChanged() ), this, SLOT( noticeChange() ) );
}

void TextObjectDialog::setupInitialValuesPrivate()
{
        fontWidget_->setFont( textObject_->font() );
}

void TextObjectDialog::castDrawObject()
{
        textObject_ = static_cast<TextObject*>( drawObject() );
}

template<>
ObjectDialog* TObjectGUIHandler<TextObject>::makeEditDialog( DrawObject* o, EditdialogAction* a,
							     QWidget* parent )
{
        return new TextObjectDialog( o, a, parent );
}


