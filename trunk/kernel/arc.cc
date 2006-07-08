 
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
        doSpecificPreparation();
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

        startAngle_ = Geom::qangle( points_[0], points_[1] );
        int endAngle = Geom::qangle( points_[0], points_[2] );

        sweepLength_ = endAngle - startAngle_;

        direction_ = ( sweepLength_ > 2880 ) ? -1 : +1;    

        if ( sweepLength_ > 2880 ) {
                direction_ = -1;
                sweepLength_ -= 5760;
        } else
                direction_ = +1;
        
        
        if ( points_.size() < 4 )
                return;

        endAngle =  Geom::qangle( points_[0], points_[3] );
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

// bool Arc::pointHitsOutline( const QPointF& p, qreal tolerance ) const
// {
//         double r1 = Geom::distance( points_[0], p );
//         double r2 = Geom::distance( points_[0], points_[1] );

// //        qDebug() << r1 << r2;

//         if ( fabs( r1-r2 ) > tolerance )
//                 return false;

//         int ang = Geom::qangle( points_[0], p );

// //        qDebug() << "angles:" << ang << startAngle_ << startAngle_ + sweepLength_ * direction_; 
        
//         return  ang > startAngle_ &&
//                 ang < startAngle_ + sweepLength_;
// }

void Arc::getReadyForDraw()
{
//        qDebug() << "getReadyForDraw" << this << points_.size();
        if ( points_.size() < 2 )
                return;
        
        setupRects();
        setupPainterPath();
}


// QPointF Arc::startAngle()
// {
//         if ( points_.size() < minPoints() )
//                 return QPointF();

//         return arrowAngle( startAngle_ );
// }

// QPointF Arc::endAngle()
// {
//         if ( points_.size() < minPoints() )
//                 return QPointF();

//         return arrowAngle( startAngle_ + sweepLength_ * direction_ );
// }

// QPointF Arc::arrowAngle( int angle )
// {
//         double a = angle * 16 / 360 * M_PI;
//         return QPointF( sin( a ), cos( a ) );
// }

