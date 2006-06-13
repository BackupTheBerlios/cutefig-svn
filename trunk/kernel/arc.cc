 
/*****************************************************************************
**
**  @version $Id: bezierspline.cc 110 2006-05-04 20:38:38Z joh $
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

#include "arc.h"
#include "outputbackend.h"
#include "geometry.h"


#include <QPainter>



Arc::Arc( Figure* parent )
        : DrawObject( parent ),
          isCircle_( true )
{
}



Arc::Arc( const Arc* other )
        : DrawObject( other ),
          isCircle_( other->isCircle_ )
{
        getReadyForDraw();
}


void Arc::outputToBackend( OutputBackend* ob )
{
        ob->outputArc( this );
}

void Arc::setupPainterPath()
{
        painterPath_ = points_.size() < 2 ? QPainterPath() : QPainterPath( points_[1] );
        
        if ( points_.size() < 3 )
                return;

        startAngle_ = Geom::qangle( Geom::angle( points_[0], points_[1] ) );
        int endAngle = Geom::qangle( Geom::angle( points_[0], points_[2] ) );

        sweepLength_ = endAngle - startAngle_;

        direction_ = ( sweepLength_ > 2880 ) ? -1 : +1;    

        if ( sweepLength_ > 2880 ) {
                direction_ = -1;
                sweepLength_ -= 5760;
        } else
                direction_ = +1;
        
        
        if ( points_.size() < 4 )
                return;

        endAngle =  Geom::qangle( Geom::angle( points_[0], points_[3] ) );
        sweepLength_ = endAngle - startAngle_;

        painterPath_.arcTo( bRect_, double(startAngle_) / 16,
                            double(sweepLength_) / 16 * direction_ );
        
}

void Arc::setupRects()
{
        double d = 2 * Geom::distance( points_[0],points_[1] );
        bRect_ = Geom::centerRect( points_[0], QSizeF( d,d ) );
        cRect_ = points_.boundingRect() | bRect_;
}

void Arc::doSpecificPreparation()
{
        
}

void Arc::drawMetaData( QPainter* p ) const
{
        switch ( points_.size() ) {
            case 2: p->drawEllipse( bRect_ ); break;
            case 3: p->drawArc( bRect_, startAngle_, sweepLength_ ); break;
            default: break;
        }
}

void Arc::passPointFlag( Fig::PointFlags f )
{
    
}

int Arc::nextPointIndex()
{
        if ( currentPointIndex() < 3 ) {
                points_ << QPointF();
                return currentPointIndex() + 1;
        }
        
        return -1;
}

bool Arc::pointHitsOutline( const QPointF& p, qreal tolerance ) const
{
        return false;
}

void Arc::getReadyForDraw()
{
        if ( points_.size() < 2 )
                return;
        
        setupPainterPath();
        setupRects();
}
