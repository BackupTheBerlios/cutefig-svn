
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
#include "objecthandler.h"
#include "geometry.h"


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
        const double c = cos( -angle_ );
        const double s = sin( -angle_ );

        const QPointF h(  w2_*c, w2_*s );
        const QPointF v( -h2_*s, h2_*c );

        const QRectF r = Geom::centerRect( p, QSizeF( tol,tol ) );

        const QPointF p1 = center_ + h + v;
        const QPointF p2 = center_ + h - v;

        if ( Geom::intersect( QLineF( p1, p2 ), r ) ) 
                return true;

        const QPointF p3 = center_ - h - v;
        if ( Geom::intersect( QLineF( p2, p3 ), r ) ) 
                return true;
        
        const QPointF p4 = center_ - h + v;
        if ( Geom::intersect( QLineF( p3, p4 ), r ) ) 
                return true;

        if ( Geom::intersect( QLineF( p4,p1 ), r ) ) 
                return true;

        return false;
}                              

void Rectangle::outputToBackend( OutputBackend* ob ) const
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
DrawObject* TObjectHandler<Rectangle>::parseObject( std::istream& is, Figure* fig )
{
        double angle;
        is >> angle;

        if ( is.fail() )
                return 0;
        
        Rectangle* r = new Rectangle( fig );
        r->setAngle( angle );

        return r;
}

