 
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

#include <cmath>
#include <QPaintEvent>
#include <QPainter>

#include <QDebug>

int Ruler::tLen_ = CuteFig::rulerWidth/2;
int Ruler::stLen_ = CuteFig::rulerWidth/4;
//int Ruler::fontSize = CuteFig::rulerWidth/3;

/** \class Ruler
 *
 *  This is the ruler for the canvas view. It shows ticks and marks
 *  them. Depending on zoom and length scale the ticks and the
 *  tickmarks are calculated in a suitable way by calcTickMarks(). It
 *  indicates the current pointerposition by a red line. It can be
 *  drawn vertically or horiziontally depending on the orientation
 *  given in the constructor.
 *
 *  It is also planned that helplines can be pulled out of the ruler.
 *
 */

Ruler::Ruler( int l, Qt::Orientation o, QWidget * parent )
        : QWidget( parent ),
          o_( o ),
          value_( 0 ),
          oldValue_( 0 ),
          scale_( 1.0 ),
          unit_( CuteFig::unit ),
          startTick_( -2 ),
          startVal_( 0 ),
          tickMarks_( 0 )
{
        setLength( l );
        setStart( 0 );
}


void Ruler::setScale( double s )
{
        scale_ = s;
        calcTickMarks();
        updateRuler();
}

void Ruler::setUnit( double u )
{
        unit_ = u;
        calcTickMarks();
        updateRuler();
}

/** Calculates the tickmarks in a way that they can be displayed properly.
 */
void Ruler::calcTickMarks()
{
        double u = unit_ * scale_;
        double t = 1/u;

        while ( u > 50.0 ) u /= 2.0;
        while ( u < 20.0 ) u *= 2.0;

        t *= u;

        ticks_ = qRound( u );
        subTicks_ = (int) ceil( u / ( (u>40.0) ? 10 : 2  ) );

        tickMarks_.clear();

        double v = startVal_ * t;
        int p = 2;
        for ( double i=0 ; i < length_; i += ticks_ ) { 
                if ( t/v < 0.1 )
                        p = 3;
                tickMarks_.append( QString::number( ( v ), 'g', p ) );
                v += t;
        }
}

/** sets the value of the pointer position and repaints the area that
 * needs to be repainted.
 */
void Ruler::setValue( int v )
{
        oldValue_ = value_;
        value_ = v - startPix_;
        if ( o_ == Qt::Horizontal ) {
                repaint( QRect( value_-5,0, 10, height() ) ); 
                repaint( QRect( oldValue_-5,0, 10, height() ) );
        } else {
                repaint( QRect( 0,value_-5, width(), 10 ) );
                repaint( QRect( 0,oldValue_-5, width(), 10 ) );
        }
}

void Ruler::paintEvent( QPaintEvent *e )
{
        QPainter p( this );
        p.setClipRect( e->rect() );

        p.drawPixmap( QPoint( 0,0 ), buffer_ );

        p.setPen( Qt::red );

        if ( o_ == Qt::Horizontal ) 
                p.drawLine( value_, 0, value_, height() );
        else 
                p.drawLine( 0, value_, width(), value_ );
}


void Ruler::setLength( int l )
{
        length_ = l;
        calcTickMarks();
        if ( o_ == Qt::Horizontal )
                resize( l, CuteFig::rulerWidth );
        else
                resize( CuteFig::rulerWidth, l );
        buffer_.resize( size() );
        updateRuler();
}

/** All the coordinates ticks are to be drawn at and so are stored in
 * pointers. This allows to draw use the same drawing loop for both
 * orientations.
 */
void Ruler::updateRuler()
{
        buffer_.fill( Qt::white );
        QPainter p( &buffer_ );
        p.setBrush( Qt::white );
        p.setPen( Qt::black );
        
        QFont font = p.font();
        font.setPixelSize( qRound( CuteFig::rulerWidth/3) );
        p.setFont( font );
    
        p.drawRect( 0,0, width(), height() );

        int *x1,*y1, *x2,*y2, *x3,*y3, *x4,*y4, *rw;

        int i  = -startTick_;
        int null = 0;
        int wh = CuteFig::rulerWidth;
        int tl = wh - tLen_;
        int stl = wh - stLen_;
        int fw;

        QRect r;
        r.setHeight( p.fontMetrics().height() );

        if ( o_ == Qt::Horizontal ) {
                x1 = x2 = x3 = &i; y1 = &wh; y2 = &tl; y3 = &stl; 
                rw = &fw; x4 = &null; y4 = &stl;
        } else {
                y1 = y2 = y3 = &i; x1 = &wh; x2 = &tl; x3 = &stl; 
                rw = &wh; x4 = &tl; y4 = &null;
        }

        int h;
        QStringList::Iterator it = tickMarks_.begin();
        while ( i<length_ && it != tickMarks_.end() ) {
                p.drawLine( QPointF(*x1,*y1), QPointF(*x2,*y2) );
                fw = p.fontMetrics().width( *it );
                r.setWidth( *rw );
                r.moveCenter( QPoint( (*x1-*x4), (*y1-*y4) ) );
                p.drawText( r, Qt::AlignHCenter || Qt::AlignVCenter, *it++ );
                h = i + ticks_;
                for ( ; i<h; i+=subTicks_ )
                        p.drawLine( QPoint(*x1,*y1), QPointF(*x3,*y3) );
                i = h;
        }

        repaint();
}


void Ruler::setStart( int v )
{
        startPix_ = v;
        startTick_ = v % ticks_ - 2;
        startVal_ = qRound( qreal(v) /ticks_);
        calcTickMarks();
        updateRuler();
}
