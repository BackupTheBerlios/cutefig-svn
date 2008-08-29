
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
#include "figure.h"
#include "outputbackend.h"
#include "objecthandler.h"
#include "geometry.h"


Rectangle::Rectangle()
        : Rectangloid(),
          xCornerRad_( -1. ),
          yCornerRad_( -1. ),
	  roundedCorners_( false ),
	  equalCornerRadii_( false )
{    
}

Rectangle::Rectangle( const Rectangle* r )
        : Rectangloid( r ),
          xCornerRad_( r->xCornerRad_ ),
          yCornerRad_( r->yCornerRad_ ),
	  roundedCorners_( r->roundedCorners_ ),
	  equalCornerRadii_( r->equalCornerRadii_ )
{
}

void Rectangle::setXCornerRad( double r )
{
	xCornerRad_ = r;
	update();
}

void Rectangle::setYCornerRad( double r )
{
	yCornerRad_ = r;
	update();
}

double Rectangle::xCornerRad() const
{
        if ( !roundedCorners_ )
		return -1.0;

	return xCornerRad_;
}

double Rectangle::yCornerRad() const
{
	if ( !roundedCorners_ )
		return -1.0;

	if ( equalCornerRadii_ )
		return xCornerRad_;

	return yCornerRad_;
}

double Rectangle::maxXCornerRad() const
{
	double max = std::fabs( points_[0].x() - points_[1].x() );

	if ( equalCornerRadii_ )
		max = qMin( max, std::fabs( points_[0].y() - points_[1].y() ) );

	return max / 2;
}	

double Rectangle::maxYCornerRad() const
{
	double max = std::fabs( points_[0].y() - points_[1].y() );

	if ( equalCornerRadii_ )
		max = qMin( max, std::fabs( points_[0].x() - points_[1].x() ) );

	return max / 2;
}

bool Rectangle::hasRoundedCorners() const
{
        return roundedCorners_;
}

bool Rectangle::hasEqualCornerRadii() const
{
        return !hasRoundedCorners() || equalCornerRadii_;
}

void Rectangle::setRoundedCorners( bool rc )
{
	roundedCorners_ = rc;
	update();
}

void Rectangle::setEqualRoundedCorners( bool eq )
{
	if ( equalCornerRadii_ == eq )
		return;
	
	equalCornerRadii_ = eq;
	if ( eq ) {
		double r = qMin( xCornerRad_, yCornerRad_ );
		yCornerRad_ = xCornerRad_ = r;
	}

	update();
}

bool Rectangle::pointHitsOutline( const QPointF& p, qreal tol ) const
{
        const double c = cos( -angleRad() );
        const double s = sin( -angleRad() );

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
        
        if ( w2_ < xCornerRad_ )
                xCornerRad_ = w2_;
        if ( h2_ < yCornerRad_ )
                yCornerRad_ = h2_;
}

void Rectangle::addPath( const QRectF& rect )
{
        QRectF r = rect;
        r.moveCenter( center_ );
	double rx = xCornerRad();
	double ry = yCornerRad();
	
        if ( !roundedCorners_  || Geom::isEqual( rx*ry, 0.0 ) )
                painterPath_.addRect( rect );
        else {
		rx *= 2;
		ry *= 2;
		
                const double l = rect.left();
                const double r = rect.right();
                const double t = rect.top();
                const double b = rect.bottom();

                const double rx = 2*xCornerRad_;
                const double ry = equalCornerRadii_ ? rx : 2*yCornerRad_;
                
                painterPath_.moveTo( l+rx/2, t );

                painterPath_.arcTo( r-rx,t, rx,ry, 90,-90 );
                painterPath_.arcTo( r-rx,b-ry, rx,ry, 0,-90 );
                painterPath_.arcTo( l,b-ry, rx,ry, -90,-90 );
                painterPath_.arcTo( l,t, rx,ry, 180,-90 );
        }
}

template<>
DrawObject* TObjectHandler<Rectangle>::parseObject( QTextStream& is, const Figure* fig )
{
        double angle, xr, yr;
        is >> angle >> xr >> yr;

        if ( is.status() != QTextStream::Ok )
                return 0;

        Rectangle* r = new Rectangle();
        r->setAngle( angle );
	r->roundedCorners_ = ! (xr < 0);
	r->equalCornerRadii_ = (yr < 0);
        r->xCornerRad_ = xr * fig->unit();
        r->yCornerRad_ = yr * fig->unit();

        return r;
}



