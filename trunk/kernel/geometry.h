 
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



#ifndef geometry_h
#define geometry_h

#include <QPointF>
#include <QRectF>
#include <QLineF>
#include <QVector>

#include <cmath>

//! Some geometrical operations
/*!
 *  Here some geometrical routines are implemented.
 */
namespace Geom
{
        //! returns the distance between two points
        qreal distance( const QPointF& p1, const QPointF& p2 );

        double angle( const QPointF& p1, const QPointF& p2 );

        //! returns a QVector of six QPointF describing the QRectF r
        QVector<QPointF> boundingPoints( const QRectF& r );

        //! returns true if the distance of p1 and p2 is "low"
        bool isNear( const QPointF& p1, const QPointF& p2 );

        //! returns true if the line l intersects the rectangle r
        bool intersect( const QLineF& l, const QRectF& r );

        //! returns the rectangle of the given size and the center.
        QRect centerRect( const QPoint& center, const QSize& size );
        QRectF centerRect( const QPointF& center, const QSizeF& size );

        //! returns the scalar product of p1 and p2
        qreal scalarProduct( const QPointF& p1, const QPointF& p2 );

        //! returns the absolute value of the point p, the length so to speak.
        qreal pabs( const QPointF& p );

        const double clickTolerance = 5;

        const double rad = 180/M_PI;

        int qangle( double angle );
};


#endif
