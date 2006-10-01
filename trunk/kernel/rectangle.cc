
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

#include "rectangle.h"
#include "outputbackend.h"
#include "objecthandler.h"
#include "geometry.h"


Rectangle::Rectangle( Figure* parent )
        : Rectangloid( parent ),
          xCornerRad_( -1. ),
          yCornerRad_( -1. )
{    
}

Rectangle::Rectangle( const Rectangle* r )
        : Rectangloid( r ),
          xCornerRad_( r->xCornerRad_ ),
          yCornerRad_( r->yCornerRad_ )
{
        getReadyForDraw();
}

void Rectangle::setXCornerRad( double r )
{
        r *= figure_->unit();
        if ( points_.size() > 2 && 2*r < std::fabs( points_[0].x() - points_[1].x() ) )
                xCornerRad_ = r;
}

void Rectangle::setYCornerRad( double r )
{
        r *= figure_->unit();
        if ( points_.size() > 2 && 2*r < std::fabs( points_[0].y() - points_[1].y() ) ) 
                yCornerRad_ = r;
}

double Rectangle::xCornerRad() const
{
        double ret = 0.0;
        
        if ( hasRoundedCorners() )
                ret = xCornerRad_ < 0 ? yCornerRad_ : xCornerRad_;

        return ret < 0 ? 0.0 : ret/figure_->unit();
}

double Rectangle::yCornerRad() const
{
        double ret = 0.0;
        
        if ( hasRoundedCorners() )
                return yCornerRad_ < 0 ? xCornerRad_ : yCornerRad_;

        return ret < 0 ? 0.0 : ret/figure_->unit();
}

double Rectangle::xCornerRadVar() const
{
        return xCornerRad_ < 0 ? -1.0 : xCornerRad_/figure_->unit();
}

double Rectangle::yCornerRadVar() const
{
        return yCornerRad_ < 0 ? -1.0 : yCornerRad_/figure_->unit();
}


bool Rectangle::hasRoundedCorners() const
{
        return ( !(xCornerRad_<0 && yCornerRad_<0) );
}

bool Rectangle::hasEqualCornerRadii() const
{
        return !hasRoundedCorners() || xCornerRad_ * yCornerRad_ < 0;
}

void Rectangle::setRoundedCorners( bool rc )
{
        if ( !(rc ^ hasRoundedCorners()) )
                return;

        if ( rc ) 
                xCornerRad_ = yCornerRad_ = .1;
        else
                xCornerRad_ = yCornerRad_ = -1.;
}

void Rectangle::setEqualRoundedCorners( bool eq )
{
        if ( !hasRoundedCorners() || !(eq ^ hasEqualCornerRadii() ) )
                return;

        if ( eq )
                if ( yCornerRad_ > xCornerRad_ ) 
                        xCornerRad_ = -1.;
                else
                        yCornerRad_ = -1.;
        else
                if ( xCornerRad_ < 0 )
                        xCornerRad_ = yCornerRad_;
                else
                        yCornerRad_ = xCornerRad_;
}

bool Rectangle::pointHitsOutline( const QPointF& p, qreal tol ) const
{
        const double c = cos( -angle_ );
        const double s = sin( -angle_ );

        const QPointF h(  w2_*c, w2_*s );
        const QPointF v( -h2_*s, h2_*c );

        const QRectF r = Geom::centerRect( p, QSizeF( tol,tol ) );

        const QPointF p1 = center_ + h + v;
        const QPointF p2 = center_ + h - v;

        if ( Geom::intersect( QLineF( p1, p2 ), r ) ) 
                return true;

        const QPointF p3 = center_ - h - v;
        if ( Geom::intersect( QLineF( p2, p3 ), r ) ) 
                return true;
        
        const QPointF p4 = center_ - h + v;
        if ( Geom::intersect( QLineF( p3, p4 ), r ) ) 
                return true;

        if ( Geom::intersect( QLineF( p4,p1 ), r ) ) 
                return true;

        return false;
}                              

void Rectangle::outputToBackend( OutputBackend* ob ) const
{
        ob->outputRectangle( this );
}

void Rectangle::setupWidthAndHeight()
{
        center_ = ( points_[0] + points_[1] ) / 2;
        w2_ = std::fabs(center_.x() - points_[1].x());
        h2_ = std::fabs(center_.y() - points_[1].y());

        qDebug() << __PRETTY_FUNCTION__ << w2_ << xCornerRad_ << h2_ << yCornerRad_;
        
        if ( w2_ < xCornerRad_ )
                xCornerRad_ = w2_;
        if ( h2_ < yCornerRad_ )
                yCornerRad_ = h2_;
}

void Rectangle::addPath()
{
        if ( ( Geom::isEqual( xCornerRad_ * yCornerRad_, 0.0 ) ) ||
             ( !hasRoundedCorners() ) )
                painterPath_.addRect( bRect_ );

        else {
                const double l = bRect_.left();
                const double r = bRect_.right();
                const double t = bRect_.top();
                const double b = bRect_.bottom();

                const double rx = xCornerRad_ > 0 ? 2*xCornerRad_ : 2*yCornerRad_;
                const double ry = yCornerRad_ > 0 ? 2*yCornerRad_ : 2*xCornerRad_;
                
                painterPath_.moveTo( l+rx/2, t );

                painterPath_.arcTo( r-rx,t, rx,ry, 90,-90 );
                painterPath_.arcTo( r-rx,b-ry, rx,ry, 0,-90 );
                painterPath_.arcTo( l,b-ry, rx,ry, -90,-90 );
                painterPath_.arcTo( l,t, rx,ry, 180,-90 );
        }
}

template<>
DrawObject* TObjectHandler<Rectangle>::parseObject( std::istream& is, Figure* fig )
{
        double angle, xr, yr;
        is >> angle >> xr >> yr;

        if ( is.fail() )
                return 0;
        
        Rectangle* r = new Rectangle( fig );
        r->setAngle( angle );
        r->xCornerRad_ = xr * fig->unit();
        r->yCornerRad_ = yr * fig->unit();

        return r;
}


