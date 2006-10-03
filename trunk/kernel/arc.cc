 
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

#include "arc.h"
#include "outputbackend.h"
#include "objecthandler.h"
#include "geometry.h"


#include <QPainter>



Arc::Arc( Figure* parent )
        : DrawObject( parent ),
          isCircle_( true ),
          rotation_()
{
}



Arc::Arc( const Arc* other )
        : DrawObject( other ),
          isCircle_( other->isCircle_ ),
	  rectSize_( other->rectSize_ ),
	  angle_( other->angle_ ),
	  arcFlag_( other->arcFlag_ ),
	  sweepFlag_( other->sweepFlag_ ),
	  center_( other->center_ ),
	  startAngle_( other->startAngle_ ),
	  sweepLength_( other->sweepLength_ ),
	  direction_( other->direction_ ),
	  rotation_( other->rotation_ )
	  
{
	setupPainterPath();
	setupRects();
}


void Arc::outputToBackend( OutputBackend* ob ) const
{
        ob->outputArc( this );
}


void Arc::setupRects()
{
	bRect_ = painterPath_.boundingRect();
	cRect_ = bRect_;
}

void Arc::doSpecificPreparation()
{
        
}

void Arc::drawMetaData( QPainter* p ) const
{
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



template<>
DrawObject* TObjectHandler<Arc>::parseObject( std::istream& is, Figure* fig )
{
        QSizeF rs;
        double angle;
        int af, sf;

        is >> rs >> angle >> af >> sf;

        if ( is.fail() )
                return 0;

        Arc* arc = new Arc( fig );
        arc->setRectSize( rs*2*fig->unit() );
        arc->setAngle( angle );
        arc->setArcFlag( af );
        arc->setSweepFlag( sf );

        return arc;
}


void Arc::setRectSize( const QSizeF& s )
{
        rectSize_.setWidth( fabs(s.width() ) );
        rectSize_.setHeight( fabs(s.height() ) );

        isCircle_ = ( rectSize_.width() == rectSize_.height() );
        update();
}


//! see http://www.w3.org/TR/SVG/implnote.html#ArcImplementationNotes
void Arc::calcCenterParameters()
{
        const QPointF& s = points_[0];
        const QPointF& e = points_[1];
 
        QPointF d = (s+e)/2;
        QMatrix mat = rotation_.inverted().translate( -d.x(), -d.y() );
        QPointF sp = s * mat;
        QPointF ep = e * mat;

        const double& spx = sp.rx(); const double& spy = sp.ry();
        double rx = rectSize_.width()/2; double ry = rectSize_.height()/2;

        double gamma = spx*spx/(rx*rx) + spy*spy/(ry*ry);
        if ( gamma > 1 ) {
                double sg = sqrt( gamma );
                rx *= sg; ry *= sg;
                rectSize_ = QSizeF( 2*rx,2*ry );
        }

        double rx2 = rx*rx; double ry2 = ry*ry;
        double rxspy = rx*spy; double ryspx = ry*spx;
        double rxspy2 = rxspy*rxspy; double ryspx2 = ryspx*ryspx;
        
        QPointF cp;
        double square = (rx2*ry2-rxspy2-ryspx2);
        if ( square > 0 ) {
                cp = sqrt( square / (rxspy2+ryspx2) ) * QPointF( rxspy/ry, -ryspx/rx );
                if ( arcFlag_ == sweepFlag_ )
                        cp *= -1;
        }

        center_ = cp * mat.inverted();
                
        startAngle_       = -atan2( (sp.y()-cp.y())/ry, (sp.x()-cp.x())/rx ) * Geom::rad;
        if ( startAngle_ < 0 )
                startAngle_ += 360;
        
        double endAngle   = -atan2( (-sp.y()-cp.y())/ry, (-sp.x()-cp.x())/rx ) * Geom::rad;
        sweepLength_ = endAngle - startAngle_;

        if ( sweepLength_ < 0 ) {
                if ( !sweepFlag_ ) 
                        sweepLength_ = 360 + sweepLength_;
        } else {
                if ( sweepFlag_ )
                        sweepLength_ = 360 - sweepLength_;
        }
}


void Arc::calcEndPointParameters()
{
        double rx = rectSize_.width()/2;
        double ry = rectSize_.height()/2;

        double th = startAngle_/Geom::rad;
        double sl = sweepLength_/Geom::rad;
        
        QPointF s = QPointF( rx*cos(th), -ry*sin(th) ) * rotation_ + center_;
        QPointF e = QPointF( rx*cos(th+sl), -ry*sin(th+sl) ) * rotation_ + center_;
}


void Arc::setupPainterPath()
{
        calcCenterParameters();
        calcEndPointParameters();
        painterPath_ = QPainterPath( rotation_.inverted().map(points_[0]-center_) );
        painterPath_.arcTo( Geom::centerRect( QPointF(), rectSize_ ), startAngle_, sweepLength_ );
        painterPath_ = rotation_.map( painterPath_ );
        painterPath_ = QMatrix().translate( center_.x(), center_.y() ).map( painterPath_ );
}

void Arc::setAngle( double a )
{
        angle_ = a;
        rotation_ = QMatrix().rotate( a );
}
