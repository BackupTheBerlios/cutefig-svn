 
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

#include <QTime>
#include <QDebug>


Ruler::Ruler( int l, int width, Qt::Orientation o, QWidget * parent )
        : QFrame( parent ),
          o_( o ),
          value_( 0 ),
          oldValue_( 0 ),
	  length_( l ),
          zoomScale_( 1.0 ),
          unit_(),
	  offset_( 0 ),
	  startPos_( 0 ),
	  rulerWidth_( width ),
          tickMarks_( 0 ),
          indicating_( false )
{
        setFrameStyle( Panel | Sunken );
        frameSpace_ = 2*frameWidth();

	setAutoFillBackground( true );
	QPalette pal = palette();
	pal.setColor( QPalette::Background, Qt::white );
	setPalette( pal );

	calcTickMarks();
}

void Ruler::setIndicating( bool indicating )
{
        indicating_ = indicating;
        update();
}

void Ruler::setZoomScale( double s )
{
        zoomScale_ = s;
        calcTickMarks();
}

void Ruler::setUnit( const ResourceKey& k )
{
        unit_.setResource( k );
        calcTickMarks();
	update();
}

void Ruler::setStart( int v )
{
	startPos_ = v;
        calcTickMarks();
	update();
}

void Ruler::setOffset( double o )
{
	offset_ = o;
	calcTickMarks();
}

/*! Calculates the tickmarks in a way that they can be displayed properly.
 */
void Ruler::calcTickMarks()
{
        double unit = unit_.data() * zoomScale_;
	tickDist_ = unit;
	
	int d = 2;
	
        while ( tickDist_ > 80.0 ) {
		tickDist_ /= d;
		d = d == 2 ? 5 : 2;
	}
	
        while ( tickDist_ < 40.0 ) {
		d = d == 2 ? 5 : 2;
		tickDist_ *= 5.0;
	}	

	double step = tickDist_/unit;

	tickDist_ = step*unit;
	
        subTickDist_ = tickDist_ / d;

        tickMarks_.clear();

        double val = -qRound( (offset_*zoomScale_-startPos_)/unit/step ) * step;
        double pos = 0;

	int prec = -(int) std::floor( std::log10(step) );
	if ( prec < 0 )
		prec = 0;
	
        do {
                tickMarks_ << QString::number( ( val ), 'f', prec );
                val += step;
                pos += tickDist_;
        } while ( pos < length_ );
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
                r1 = QRect( value_-1,0, 3, height() );
                r2 = QRect( oldValue_-1,0, 3, height() );
        } else {
		r1 = QRect( 0,value_-frameWidth()-1, width(), 3 );
		r2 = QRect( 0,oldValue_-frameWidth()-1, width(), 3 );
	}

        update( QRegion(r1) | QRegion(r2) );
}


inline bool rect_intersects(const QRect &r1, const QRect &r2)
{
    return qMax(r1.left(), r2.left()) <= qMin(r1.right(), r2.right())
        && qMax(r1.top(), r2.top()) <= qMin(r1.bottom(), r2.bottom());
}


/*!
    \since 4.2

    Returns true if this region intersects with \a rect, otherwise
    returns false.
*/
bool regionrect_intersects(const QRegion& reg, const QRect &rect)
{
    QRect r = rect.normalized();
    const QVector<QRect> myRects = reg.rects();
    for (QVector<QRect>::const_iterator it = myRects.begin(); it < myRects.end(); ++it)
        if (rect_intersects(r, *it))
            return true;
    return false;
}


void Ruler::paintEvent( QPaintEvent* e )
{
        QPainter p( this );
        p.setClipRegion( e->region() );

	if ( o_ == Qt::Vertical ) {
		p.rotate( -90 );
		p.translate( -length_-frameSpace_,0 );
	}
	QMatrix matr = p.matrix().inverted();
	
        p.translate( frameWidth(), frameWidth() );

	
 	QFont font = p.font();
 	font.setPixelSize( qRound( rulerWidth_/3 ) );
 	p.setFont( font );

	
	const double d = (offset_*zoomScale_-startPos_)/tickDist_;
	double pos = ( d - qRound(d) ) * tickDist_;
	const double rw   = rulerWidth_;
	const double rw2  = rulerWidth_/2;
	const double rw4  = rulerWidth_/4;
	const double rw34 = rw4*3;
	
	int sign = 1;
 	if ( o_ == Qt::Vertical ) {
 		sign = -1;
 		pos = length_ - pos;
 	}

	int stop = o_ == Qt::Horizontal ? width() :  height()-qRound(pos);
	QRegion reg = p.matrix().inverted().map( e->region() );
	
	foreach ( QString tm, tickMarks_ ) {
		p.drawLine( QPointF(pos,rw), QPointF(pos,rw2) );
		QSize s = p.fontMetrics().size( Qt::TextSingleLine, tm );
		QRect r = Geom::centerRect( QPointF(pos,rw4).toPoint(), s );
// Qt42		if ( e->region().intersects( r ) )
		if ( regionrect_intersects( reg, r ) ) {
			p.setClipRegion( p.clipRegion() | r );
			p.drawText( r, Qt::AlignHCenter | Qt::AlignVCenter, tm );
		}
		
		for ( double j=0; j+subTickDist_/2<tickDist_; j+=subTickDist_ ) {
			pos += subTickDist_*sign;
			p.drawLine( QPointF(pos,rw), QPointF(pos,rw34) );
		}

		if ( pos*sign > stop )
			break;

	}
	
	if ( value_ < length_ && indicating_ ) {
                p.setPen( Qt::red );
		int v = o_ == Qt::Vertical ? length_-value_+frameWidth() : value_;
		p.drawLine( v, 0, v, rulerWidth_ );
        }
	
	p.end();

        QFrame::paintEvent( e );
}

void Ruler::setLength( int l )
{
        length_ = l;
        
        calcTickMarks();

        update();
}

void Ruler::contextMenuEvent( QContextMenuEvent* )
{
}





RulerDispatcher::RulerDispatcher( const QSize& s, QObject* parent )
	: QObject( parent ),
          rulerWidth_( 30 ),
	  vertical_( new Ruler( s.height(), rulerWidth_, Qt::Vertical ) ),
	  horizontal_( new Ruler( s.width(), rulerWidth_, Qt::Horizontal ) ),
          unitLabel_( new QLabel( ResLib<Length>::defaultKey().keyString() ) )
{
        unitLabel_->setFixedSize( QSize( rulerWidth_, rulerWidth_ ) );
}

void RulerDispatcher::setUnit( const ResourceKey& k )
{
        unitLabel_->setText( k.keyString() );
        vertical_->setUnit( k );
        horizontal_->setUnit( k );
}

void RulerDispatcher::sizeChange( const QSize& s )
{
	vertical_->setLength( s.height() );
	horizontal_->setLength( s.width() );
}

void RulerDispatcher::setMatrix( const QMatrix& m )
{
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
