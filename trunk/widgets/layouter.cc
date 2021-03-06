 
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

#include "layouter.h"

#include <QString>
#include <QLayout>
#include <QLabel>

#include <QDebug>


Layouter::Layouter( QBoxLayout* layout )
        : layout_( layout )
{
}

Layouter& Layouter::labeledWidget( const QString& text, QWidget* widget, int stretch )
{
        QLabel* label = new QLabel( text, widget->parentWidget() );
        label->setBuddy( widget );
        layout_->addWidget( label );
        layout_->addWidget( widget, stretch );

        return *this;
}

Layouter& Layouter::widget( QWidget* w, int stretch )
{
        layout_->addWidget( w, stretch );
        return *this;
}

Layouter& Layouter::stretch( int stretch )
{
        layout_->addStretch( stretch );
        return *this;
}

void Layouter::finishTo( QBoxLayout* target )
{
        target->addLayout( layout_ );
}

void Layouter::finishTo( QBoxLayout* target, int index )
{
        target->insertLayout( index, layout_ );
}

void Layouter::finishTo( QGridLayout* target, int r, int c, int rs, int cs, Qt::Alignment algn )
{
        target->addLayout( layout_, r, c, rs, cs, algn );
}

void Layouter::finishTo( QWidget* target )
{
        target->setLayout( layout_ );
}
