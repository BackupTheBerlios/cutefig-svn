 
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

#include "rectangloid.h"
#include "geometry.h"

Rectangloid::Rectangloid( Figure* parent )
        : DrawObject( parent ),
          angle_( 0.0 )
{   
}

Rectangloid::Rectangloid( const Rectangloid* r )
        : DrawObject( r ),
          angle_( r->angle()/rad )
{
}

QPointF* Rectangloid::nextPoint()
{
        if ( points_.size() == 1 ) {
                points_.resize( 2 );
                return &points_[1];
        } else 
                return 0;
}

void Rectangloid::setupRects()
{
        double c = cos( angle_ );
        double s = sin( angle_ );

        double wc = w_*c;
        double ws = w_*s;
        double hc = h_*c;
        double hs = h_*s;
        
        double w = sqrt( wc*wc + hs*hs );
        double h = sqrt( ws*ws + hc*hc );

        cRect_ = Geom::centerRect( center_, QSizeF( w,h ) );
        bRect_ = Geom::centerRect( center_, QSizeF( w+pen_.width(), h+pen_.width() ) );
}

void Rectangloid::setupPainterPath()
{
        setupWidthAndHeight();

        w_ = 2*w2_; h_ = 2*h2_;

        bRect_.setSize( QSizeF( w_, h_ ) );
        bRect_.moveCenter( QPointF(0.,0.) );

        painterPath_ = QPainterPath();
        
        addPath();
        
        rotation_ = QMatrix();
        rotation_.rotate( -angle_*rad );        

        bRect_ = rotation_.mapRect( bRect_ );

        QMatrix rottrans = QMatrix();
        rottrans.translate( center_.x(), center_.y() );
        painterPath_ = rottrans.map( rotation_.map(painterPath_) );

}

void Rectangloid::passPointFlag( Fig::PointFlag f )
{
    
}

void Rectangloid::setAngle( double a )
{
        angle_ = a/rad;
        getReadyForDraw();
}

void Rectangloid::setAngle( int a )
{
        setAngle( (double) a );
}

void Rectangloid::setAngleNew( double a )
{
        angle_ = a/rad;
}

