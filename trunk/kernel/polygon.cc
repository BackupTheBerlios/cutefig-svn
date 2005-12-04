 
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


#include "polygon.h"
#include "outputbackend.h"
#include "geometry.h"

#include <QPainter>
#include <QPixmap>
#include <QPainter>

Polygon::Polygon( Figure* parent )
        : Polyline( parent )
{
}

Polygon::Polygon( const Polygon* o )
        : Polyline( o )
{
        getReadyForDraw();
}

void Polygon::outputToBackend( OutputBackend* ob )
{
        ob->outputPolygon( this );
}

void Polygon::setupPainterPath()
{
        Polyline::setupPainterPath();
        painterPath_.lineTo( points_[0] );
}

bool Polygon::pointHitsOutline( const QPointF& p, qreal tolerance ) const
{
        if ( Polyline::pointHitsOutline( p, tolerance ) )
                return true;

        QRectF r = Geom::centerRect( p, QSizeF( tolerance, tolerance ) );
        return Geom::intersect( QLineF( points_.first(), points_.last() ), r );
}



