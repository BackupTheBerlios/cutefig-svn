 
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

#include <QPainter>
#include <QPixmap>

#include <QDebug>

#include "geometry.h"
#include "ellipse.h"
#include "outputbackend.h"

 
Ellipse::Ellipse( Figure* parent ) 
        : DrawObject( parent )
{          
        specByRadii_ = false;
        circle_ = false;
        angle_ = 0.0;
}

Ellipse::Ellipse( Ellipse *o ) 
        : DrawObject( o ),
          specByRadii_( o->specByRadii_ ),
          circle_( o->circle_ ),
          angle_( o->angle()/rad )
{
        getReadyForDraw();
        doSpecificPreparation();
}

DrawObject* Ellipse::copy()
{
        return new Ellipse( this );
}

QPointF* Ellipse::nextPoint()
{
        if ( points_.size() == 1 ) {
                points_.resize( 2 );
                return &points_[1];
        } else 
                return 0;
}

void Ellipse::getReadyForDraw()
{
        if ( specByRadii_ )
                center_ = points_[0];
        else
                center_ = QPointF( (points_[0].x() + points_[1].x())/2,
                                   (points_[0].y() + points_[1].y())/2 );
        if ( circle_ ) {
                double d =  Geom::distance( center_, points_[1] );
                w2_ = h2_ = d; 
        } else {
                w2_ = fabs(center_.x() - points_[1].x());
                h2_ = fabs(center_.y() - points_[1].y());
        }

        w_ = 2*w2_; h_ = 2*h2_;
        

        bRect_.setSize( QSizeF( w_, h_ ) );
        bRect_.moveCenter( QPointF(0.,0.) );

        painterPath_ = QPainterPath();
        painterPath_.addEllipse( bRect_ );

        rotation_ = QMatrix();
        rotation_.rotate( -angle_*rad );        

        bRect_ = rotation_.mapRect( bRect_ );

        QMatrix rottrans = QMatrix();
        rottrans.translate( center_.x(), center_.y() );
        painterPath_ = rottrans.map( rotation_.map(painterPath_) );

        bRect_.moveCenter( center_ );

        drawRect_ = bRect_.toRect();
        addSelPointsToDrawRect();
}

void Ellipse::doSpecificPreparation()
{
        qreal angleShift = 0;
        
        qreal fpdist = w2_*w2_ - h2_*h2_;
        if ( fpdist < 0 ) {
                angleShift = M_PI_2;
                fpdist = -fpdist;
                rad_d_ = h_;
        } else
                rad_d_ = w_;

        fpdist = sqrt( fpdist );

        QPointF d( fpdist * cos( -angle_ + angleShift ), fpdist * sin( -angle_ + angleShift ) );

        fp1_ = center_ + d;
        fp2_ = center_ - d;
}

bool Ellipse::pointHitsOutline( const QPointF& p, qreal tolerance ) const
{
        qreal d = Geom::distance( p, fp1_ ) + Geom::distance( p, fp2_ );
        
        if ( brush_.style() != Qt::NoBrush )
                return d <= rad_d_; 

        d -= rad_d_;
        return fabs( d ) <= tolerance;
}

void Ellipse::setSpecByRadii( bool r )
{
        specByRadii_ = r;
        emit( objectChanged() );
}

void Ellipse::setIsCircle( bool c )
{
        circle_ = c;
        emit( objectChanged() );
}

void Ellipse::passPointFlag( Fig::PointFlag f )
{
        circle_ = circle_ || ( f & Fig::Straight );
        specByRadii_ = specByRadii_ || ( f & Fig::Special2 );
}

void Ellipse::outputToBackend( OutputBackend* ob )
{
        ob->outputEllipse( this );
}

void Ellipse::setAngle( double a )
{
        angle_ = a/rad;
        getReadyForDraw();
}

void Ellipse::setAngle( int a )
{
        setAngle( (double) a );
}

void Ellipse::setAngleNew( double a )
{
        angle_ = a/rad;
}
