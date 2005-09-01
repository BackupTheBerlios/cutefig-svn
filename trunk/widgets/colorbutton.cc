 
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

#include "colorbutton.h"
#include "strokeiconengines.h"

#include <QIcon>
#include <QColorDialog>

ColorButton::ColorButton( const QColor& c, QWidget * parent )
        : QPushButton( parent )
{
        setColor( c );
        connect( this, SIGNAL( clicked() ), this, SLOT( changeColor() ) );
}

void ColorButton::setColor( const QColor& c )
{
        color_ = c;
        setIcon( QIcon( new ColorIconEngine( color_ ) ) );
}

void ColorButton::changeColor()
{
        bool ok;
        QRgb rgb = QColorDialog::getRgba( color_.rgba(), &ok );
        if ( ok ) {
                color_ = QColor( rgb );
                color_.setAlpha( qAlpha( rgb ) );
                setColor( color_ );
                emit( colorChanged( color_ ) );
        }
}
