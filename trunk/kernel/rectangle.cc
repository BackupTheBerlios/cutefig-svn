 
/*****************************************************************************
**
**  $Id$
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

#include "rectangle.h"
#include "outputbackend.h"
#include "dobjectfactory.h"


Rectangle::Rectangle( Figure* parent )
        : Rectangloid( parent )
{    
}

Rectangle::Rectangle( const Rectangle* r )
        : Rectangloid( r )
{
        getReadyForDraw();
}

bool Rectangle::pointHitsOutline( const QPointF& p, qreal tol ) const
{
        qreal x = p.x();
        qreal y = p.y();

        qreal l = bRect_.left();
        qreal r = bRect_.right();
        qreal t = bRect_.top();
        qreal b = bRect_.bottom();
        
        return  ( ( fabs(l-x) < tol || fabs(r-x) < tol ) &&
                  ( y > t && y < b ) )
                ||
                ( ( fabs(t-y) < tol || fabs(b-y) < tol ) &&
                  ( x > l && x < r ) );
}               

void Rectangle::outputToBackend( OutputBackend* ob )
{
        ob->outputRectangle( this );
}

void Rectangle::setupWidthAndHeight()
{
        center_ = ( points_[0] + points_[1] ) / 2;
        w2_ = fabs(center_.x() - points_[1].x());
        h2_ = fabs(center_.y() - points_[1].y());
}

void Rectangle::addPath()
{
        painterPath_.addRect( bRect_ );
}

template<>
DrawObject* TDrawObjectFactory<Rectangle>::parseObject( std::istream&, Figure* fig )
{
        return new Rectangle( fig );
}
