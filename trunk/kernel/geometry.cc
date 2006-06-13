 
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

#include "geometry.h"

#include <QDebug>

qreal Geom::distance( const QPointF& p1, const QPointF& p2 )
{
        qreal xd = p1.x() - p2.x();
        qreal yd = p1.y() - p2.y();
        return hypot( xd, yd );
}

double Geom::angle( const QPointF& p1, const QPointF& p2 )
{
        return -atan2( p2.y() - p1.y(), p2.x() - p1.x() );
}


/*! Basically needed to show the scaling handles to the user. The
 *  points returned are the four corner points and the four points in
 *  the middle of the edge lines of the rectangle r if the edge line
 *  is non zero. The points are filled up in the following way:
 *    - top left
 *    - bottom right
 *    - top right
 *    - bottom left
 *    - left middle
 *    - right middle
 *    - top middle
 *    - bottom middle
 *
 *  In case the rectangle is a line (width() * height() == 0) the
 *  latter four are filled up with QPoint().
 *
 *  This behaviour is documented here in such detail, as it is
 *  mandatory to keep it that way as long as clients rely on it.
 */
QVector<QPointF> Geom::boundingPoints( const QRectF& r )
{
        QVector<QPointF> pts;
        pts << r.topLeft()
            << r.bottomRight()
            << r.topRight()
            << r.bottomLeft();

        if ( ( r.bottom() != r.top() ) && ( r.right() != r.left() ) ) {
                qreal mid = ( r.top() + r.bottom() ) / 2;
                pts << QPointF( r.left(), mid )
                    << QPointF( r.right(), mid );
                mid = ( r.left() + r.right() ) / 2;
                pts << QPointF( mid, r.top() )
                    << QPointF( mid, r.bottom() );
        } else
                pts << QPointF() << QPointF() << QPointF() << QPointF();
         
        return pts;
}

/*! The hardcoded 5 is not nice :(
 */
bool Geom::isNear( const QPointF& p1, const QPointF& p2 )
{
        return ( distance( p1, p2 ) < 5 );
}

/*! Used for example to detect whether the mouse cursor is near a line.
 */
bool Geom::intersect( const QLineF& l, const QRectF& r ) 
{
        QLineF d1( r.topLeft(), r.bottomRight() );
        QLineF d2( r.topRight(), r.bottomLeft() );
        return ( l.intersect( d1, 0 ) == QLineF::BoundedIntersection ) ||
               ( l.intersect( d2, 0 ) == QLineF::BoundedIntersection );
}

QRect Geom::centerRect( const QPoint& center, const QSize& size )
{
        QRect r;
        r.setSize( size );
        r.moveCenter( center );

        return r;
}

/*!
 * \overload QRect Geom::centerRect( const QPoint& center, const QSize& size )
 */
QRectF Geom::centerRect( const QPointF& center, const QSizeF& size )
{
        QRectF r;
        r.setSize( size );
        r.moveCenter( center );

        return r;
}

qreal Geom::scalarProduct( const QPointF& p1, const QPointF& p2 )
{
        return p1.x()*p2.x() + p1.y()*p2.y();
}

qreal Geom::pabs( const QPointF& p )
{
        return hypot( p.x(), p.y() );
}


int Geom::qangle( double angle )
{
        int qa = rint( angle / M_PI * 2880 );
        if ( qa < 0 )
                qa += 5760;

        return qa;
}
