 
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


/** \namespace Geom
 *  \brief Some simple geometrical functions that Qt does not have.
 */

#ifndef geometry_h
#define geometry_h

#include <QPointF>
#include <QRectF>
#include <QLineF>
#include <QVector>

namespace Geom
{
        qreal distance( const QPointF& p1, const QPointF& p2 );

        QVector<QPointF> boundingPoints( const QRectF& r );

        bool isNear( const QPointF& p1, const QPointF& p2 );
        bool intersect( const QLineF& l, const QRectF& r );

        QRect centerRect( const QPoint& center, const QSize& size );
        QRectF centerRect( const QPointF& center, const QSizeF& size );
};


#endif
