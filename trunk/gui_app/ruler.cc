 
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


#include "ruler.h"
#include "cutefig.h"
#include "geometry.h"

#include <cmath>
#include <QPaintEvent>
#include <QPainter>

#include <QDebug>


Ruler::Ruler( int l, Qt::Orientation o, QWidget * parent )
        : QFrame( parent ),
          o_( o ),
          value_( 0 ),
          oldValue_( 0 ),
          zoomScale_( 1.0 ),
          unit_(),
	  rulerWidth_( 30 ),
          tickMarks_( 0 ),
          indicating_( false )
{
        setFrameStyle( Panel | Sunken );
        setLineWidth( 2 );
        setLength( l );
        setStart( 0 );
}

void Ruler::setIndicating( bool indicating )
{
        indicating_ = indicating;
        update();
}

void Ruler::setZoomScale( double s )
{
	qDebug() << "Ruler::setZoomScale" << s;
        zoomScale_ = s;
        calcTickMarks();
        updateRuler();
}

void Ruler::setUnit( const ResourceKey& k )
{
        unit_.setResource( k );
        calcTickMarks();
        updateRuler();
}

/*! Calculates the tickmarks in a way that they can be displayed properly.
 */
void Ruler::calcTickMarks()
{
        double unit = unit_.data() * zoomScale_;
	double u = unit;
        double t = 1/u;

        while ( u > 50.0 ) u /= 2.0;
        while ( u < 20.0 ) u *= 2.0;

        t *= u;

        ticks_ = u;
        subTicks_ = u / ( (u>40.0) ? 5 : 2  );

        tickMarks_.clear();

	qDebug() << __PRETTY_FUNCTION__ << zoomScale_ << unit << offset_-startPos_ << qRound( offset_/unit );
        double v = -qRound( (offset_-startPos_)/unit/t ) * t;
        int p = 2;
        double i = 0;

        do {
                if ( t/v < 0.1 )
                        p = 3;
                tickMarks_ << QString::number( ( v ), 'g', p );
                v += t;
                i += ticks_;
        } while ( i < length_ );
}

/*! sets the value of the pointer position and repaints the area that
 * needs to be repainted.
 */
void Ruler::setValue( int v )
{
        oldValue_ = value_;
        value_ = v - qRound( startPos_ );

        QRect r1, r2;
        
        if ( o_ == Qt::Horizontal ) {
                r1 = QRect( value_,0, 3, height() );
                r2 = QRect( oldValue_,0, 3, height() );
        } else {
		r1 = QRect( 0,value_-2, width(), 3 );
		r2 = QRect( 0,oldValue_-2, width(), 3 );
	}

        update( QRegion(r1) | QRegion(r2) );
}

void Ruler::paintEvent( QPaintEvent *e )
{
        QPainter p( this );
        p.setClipRegion( e->region() );

	if ( o_ == Qt::Vertical ) {
		p.rotate( -90 );
		p.translate( -length_,0 );
	}
	
//	p.translate( 2,0 );
        p.drawPixmap( QPoint( 0,0 ), buffer_ );

        if ( value_ < length_ && indicating_ ) {
                p.setPen( Qt::red );
		int v = o_ == Qt::Vertical ? length_-value_ : value_;
		p.drawLine( v, 0, v, rulerWidth_ );
        }
}

void Ruler::setLength( int l )
{
        length_ = l;
        calcTickMarks();
        if ( o_ == Qt::Horizontal )
                resize( l, rulerWidth_ );
        else
                resize( rulerWidth_, l );
        buffer_ = QPixmap( l, rulerWidth_ );
        updateRuler();
}



void Ruler::updateRuler() 
{
	buffer_.fill();

	QPainter p( &buffer_ );
 	QFont font = p.font();
 	font.setPixelSize( qRound( rulerWidth_/3 ) );
 	p.setFont( font );

	const double d = (offset_-startPos_)/ticks_;
	double pos = ( d - qRound(d) ) * ticks_;
	const double rw   = rulerWidth_;
	const double rw2  = rulerWidth_/2;
	const double rw4  = rulerWidth_/4;
	const double rw34 = rw4*3;

	int sign = 1;
 	if ( o_ == Qt::Vertical ) {
 		sign = -1;
 		pos = length_ - pos;
 	}

	foreach ( QString tm, tickMarks_ ) {
		p.drawLine( QPointF(pos,rw), QPointF(pos,rw2) );
		QSizeF s = p.fontMetrics().size( Qt::TextSingleLine, tm );
		QRectF r = Geom::centerRect( QPointF(pos,rw4), s );
		p.drawText( r, Qt::AlignHCenter | Qt::AlignVCenter, tm );

		for ( double j=0; j<ticks_; j+=subTicks_ ) {
			pos += subTicks_*sign;
			p.drawLine( QPointF(pos,rw), QPointF(pos,rw34) );
		}
	}

	update();
}
	


void Ruler::setStart( int v )
{
	startPos_ = v;
        calcTickMarks();
        updateRuler();
}

void Ruler::setOffset( double o )
{
	offset_ = o;
	calcTickMarks();
	updateRuler();
}


void Ruler::contextMenuEvent( QContextMenuEvent* )
{
}





RulerDispatcher::RulerDispatcher( const QSize& s, QWidget* parent )
	: QObject( parent ),
	  vertical_( new Ruler( s.height(), Qt::Vertical, parent ) ),
	  horizontal_( new Ruler( s.width(), Qt::Horizontal, parent ) )
{
}

void RulerDispatcher::sizeChange( const QSize& s )
{
	vertical_->setLength( s.height() );
	horizontal_->setLength( s.width() );
}

void RulerDispatcher::setMatrix( const QMatrix& m )
{
	qDebug() << __PRETTY_FUNCTION__ << m;
	vertical_->setZoomScale( 1/m.m22() );
	vertical_->setOffset( -m.dy() );

	horizontal_->setZoomScale( 1/m.m11() );
	horizontal_->setOffset( -m.dx() );
}

void RulerDispatcher::setPos( const QPoint& p )
{
	vertical_->setValue( p.y() );
	horizontal_->setValue( p.x() );
}

void RulerDispatcher::setIndicating( bool i )
{
	vertical_->setIndicating( i );
	horizontal_->setIndicating( i );
}

void RulerDispatcher::verticalScroll( int s )
{
	vertical_->setStart( s );
}

void RulerDispatcher::horizontalScroll( int s )
{
	horizontal_->setStart( s );
}
