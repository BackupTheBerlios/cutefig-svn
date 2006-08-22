 
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
#include "objecthandler.h"
 
Ellipse::Ellipse( Figure* parent ) 
        : Rectangloid( parent ),
          specByRadii_( false ),
          circle_( false )
{
}

Ellipse::Ellipse( const Ellipse *o ) 
        : Rectangloid( o ),
          specByRadii_( o->specByRadii_ ),
          circle_( o->circle_ )
{
        getReadyForDraw();
        doSpecificPreparation();
}

// DrawObject* Ellipse::copy()
// {
//         return new Ellipse( this );
// }

void Ellipse::setupWidthAndHeight()
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
}

void Ellipse::addPath()
{
        painterPath_.addEllipse( bRect_ );
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
        
        if ( fill_ )
                return d <= rad_d_; 

        d -= rad_d_;
        return fabs( d ) <= tolerance;
}

void Ellipse::setSpecByRadii( bool r )
{
        specByRadii_ = r;
//         emit( objectChanged() );
}

void Ellipse::setIsCircle( bool c )
{
        circle_ = c;
//         emit( objectChanged() );
}

void Ellipse::passPointFlag_private( Fig::PointFlags f )
{
        circle_ = circle_ || ( f & Fig::Straight );
        specByRadii_ = specByRadii_ || ( f & Fig::Special2 );
}

void Ellipse::outputToBackend( OutputBackend* ob ) const
{
        ob->outputEllipse( this );
}


///


template<>
DrawObject* TObjectHandler<Ellipse>::parseObject( std::istream& is, Figure* fig )
{
        int circle, byRadius;
        double angle;

        is >> circle >> byRadius >> angle;
        
        if ( is.fail() )
                return 0;
        
        Ellipse *e = new Ellipse( fig );
        e->setIsCircle( circle );
        e->setSpecByRadii( byRadius );
        e->setAngleNew( angle );
        
        return e;
}
