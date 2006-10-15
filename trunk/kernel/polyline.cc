 
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


#include "polyline.h"
#include "outputbackend.h"
#include "geometry.h"

#include <QPainter>
#include <QPixmap>
#include <QPolygonF>

#include <cmath>

#include <QDebug>

Polyline::Polyline( Figure* parent )
        : DrawObject( parent ),
          finished_( false )
{
}

Polyline::Polyline( const Polyline* o )
        : DrawObject( o ),
          finished_( true )
{
}

void Polyline::passPointFlag( Fig::PointFlags f )
{
        if ( f & Fig::Final )
                finished_ = true;
}

int Polyline::nextPointIndex()
{       
        if (  finished_ )
                return -1;

        int s = points_.size();
        points_ << QPointF();
        return s;
}

void Polyline::setupPainterPath()
{
        painterPath_ = QPainterPath();
        painterPath_.addPolygon( points_ );        
}

bool Polyline::pointHitsOutline( const QPointF& p, qreal tolerance ) const 
{
        QRectF r = Geom::centerRect( p, QSizeF( tolerance, tolerance ) );

        QPolygonF::const_iterator it = points_.begin();
        QPointF lp = points_.first();
        while ( ++it != points_.end() ) {
                if ( Geom::intersect( QLineF( lp, *it ) , r ) )
                        return true;
                lp = *it;
        }

        return false;
}

void Polyline::outputToBackend( OutputBackend* ob ) const
{
        ob->outputPolyline( this );
}


