 
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

Polyline::Polyline( Figure * parent )
        : DrawObject( parent )
{
        nextPoint_ = &points_[0];
}

Polyline::Polyline( Polyline *o )
        : DrawObject( o ),
          nextPoint_( 0 )
{
        getReadyForDraw();
}

DrawObject * Polyline::copy()
{
        return new Polyline( this );
}

void Polyline::passPointFlag( Fig::PointFlag f )
{
        if ( f & Fig::Final )
                nextPoint_ = 0;
}

QPointF* Polyline::nextPoint()
{       
        if (  nextPoint_ ) {
                int c = points_.size();
                points_.resize( c + 1 );
                points_[c] = QPointF( 0,0 );
                nextPoint_ = &points_[c];
        }

        return nextPoint_;
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

void Polyline::outputToBackend( OutputBackend* ob )
{
        ob->outputPolyline( this );
}


