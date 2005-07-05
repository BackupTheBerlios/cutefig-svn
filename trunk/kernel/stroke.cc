 
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


#include "stroke.h"

#include <QPainter>
#include <QPainterPath>
#include <QColor>
#include <QBrush>

#include <QDebug>

Stroke::Stroke()
        : type_( None ),
          data_( QVariant() ),
          key_()
{
}

Stroke::Stroke( const Stroke& other )
        : type_( other.type_ ),
          data_( other.data_ ),
          key_( other.key_ )
{    
}

Stroke::Stroke( const QColor& color )
{
        type_ = Color;
        data_ = QVariant( color );
}

void Stroke::setColor( const QColor& color )
{
        type_ = Color;
        data_ = QVariant( color );
}

void Stroke::setSetGradient( const QGradient& gradient )
{
        type_ = Gradient;
        data_.setValue( QBrush( gradient ) );
}

void Stroke::setPixmap( const QPixmap& pixmap )
{
        type_ = Pixmap;
        data_.setValue( pixmap );
}

void Stroke::fillPath( const QPainterPath& path, QPainter* painter ) const
{
        switch ( type_ ) {
            case Color:
                    painter->fillPath( path, QBrush( data_.value<QColor>() ) );
                    break;
            case Gradient:
                    painter->fillPath( path, data_.value<QBrush>() );
                    break;
            case Pixmap:
                    painter->fillPath( path, QBrush( data_.value<QPixmap>() ) );
                    break;
            case None:
            default: break;
        }
}

