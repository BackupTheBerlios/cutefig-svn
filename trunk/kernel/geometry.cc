 
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

#include "geometry.h"
#include <cmath>

#include <QDebug>

/** Returns the distance between two points p1 and p2.
 */
qreal Geom::distance( const QPointF& p1, const QPointF& p2 )
{
        qreal xd = p1.x() - p2.x();
        qreal yd = p1.y() - p2.y();
        return sqrt( xd*xd + yd*yd );
}

/** Returns a QVector of eight points describing the rectangle
 *  r. These are the four corner points and the four points in the
 *  middle of the edges. If one of the edges has a length of 0, those
 *  are undefined.
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

/** still needed. I don't beleive so.
 */
bool Geom::isNear( const QPointF& p1, const QPointF& p2 )
{
        return ( distance( p1, p2 ) < 5 );
}

/** Returns true if the line l intersects with the rectangle r, false
 *  if not.
 */
bool Geom::intersect( const QLineF& l, const QRectF& r ) 
{
        QLineF d1( r.topLeft(), r.bottomRight() );
        QLineF d2( r.topRight(), r.bottomLeft() );
        return ( l.intersect( d1, 0 ) == QLineF::BoundedIntersection ) ||
               ( l.intersect( d2, 0 ) == QLineF::BoundedIntersection );
}

/** Returns a rectangle of the size size and the center point center.
 */
QRect Geom::centerRect( const QPoint& center, const QSize& size )
{
        QRect r;
        r.setSize( size );
        r.moveCenter( center );

        return r;
}

/** The same as above
 */
QRectF Geom::centerRect( const QPointF& center, const QSizeF& size )
{
        QRectF r;
        r.setSize( size );
        r.moveCenter( center );

        return r;
}
