 
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

Ruler::Ruler( int l, Qt::Orientation o, QWidget * parent )
        : QFrame( parent ),
          o_( o ),
          value_( 0 ),
          oldValue_( 0 ),
          scale_( 1.0 ),
          unit_(),
          startTick_( -2 ),
          startVal_( 0 ),
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

void Ruler::setScale( double s )
{
        scale_ = s;
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
        double u = unit_.data() * scale_;
        double t = 1/u;

        while ( u > 50.0 ) u /= 2.0;
        while ( u < 20.0 ) u *= 2.0;

        t *= u;

        ticks_ = u;
        subTicks_ = u / ( (u>40.0) ? 5 : 2  );

        tickMarks_.clear();

        double v = 0;//startVal_ * t;
        int p = 2;
        double i = 0;

        do {
                if ( t/v < 0.1 )
                        p = 3;
                tickMarks_.append( QString::number( ( v ), 'g', p ) );
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
        value_ = v - qRound( startPix_ );

        QRect r1, r2;
        
        if ( o_ == Qt::Horizontal ) {
                r1 = QRect( value_,0, 3, height() );
                r2 = QRect( oldValue_,0, 3, height() );
        } else {
                r1 = QRect( 0,value_, width(), 3 );
                r2 = QRect( 0,oldValue_, width(), 3 );
                
        }

        update( QRegion(r1) | QRegion(r2) );
}

void Ruler::paintEvent( QPaintEvent *e )
{
        QPainter p( this );
        p.setClipRect( e->rect() );

        if ( o_ == Qt::Horizontal )
                p.translate( 2, 0 );
        else
                p.translate( 0, 2 );

        p.drawPixmap( QPoint( 0,0 ), buffer_ );

        if ( value_ < length_ && indicating_ ) {               

                p.setPen( Qt::red );

                if ( o_ == Qt::Horizontal ) 
                        p.drawLine( value_, 0, value_, height() );
                else 
                        p.drawLine( 0, value_, width(), value_ );
        }
}

void Ruler::setLength( int l )
{
        length_ = l;
        calcTickMarks();
        if ( o_ == Qt::Horizontal )
                resize( l, CuteFig::rulerWidth );
        else
                resize( CuteFig::rulerWidth, l );
        buffer_ = QPixmap( size() );
        updateRuler();
}

/*! All the coordinates ticks are to be drawn at and so are stored in
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

        double *x1,*y1, *x2,*y2, *x3,*y3, *x4,*y4, *rw;

        double i  = -startTick_;
        double null = 0.0;
        double wh = CuteFig::rulerWidth;
        double tl = wh - tLen_;
        double stl = wh - stLen_;
        double fw;

        QRectF r;
        r.setHeight( p.fontMetrics().height() );

        if ( o_ == Qt::Horizontal ) {
                x1 = x2 = x3 = &i; y1 = &wh; y2 = &tl; y3 = &stl; 
                rw = &fw; x4 = &null; y4 = &stl;
        } else {
                y1 = y2 = y3 = &i; x1 = &wh; x2 = &tl; x3 = &stl; 
                rw = &wh; x4 = &tl; y4 = &null;
        }

        double h;
        QStringList::Iterator it = tickMarks_.begin();
        while ( i<length_ && it != tickMarks_.end() ) {
                p.drawLine( QPointF(*x1,*y1), QPointF(*x2,*y2) );
                fw = p.fontMetrics().width( *it );
                r.setWidth( qRound(*rw) );
                r.moveCenter( QPointF( (*x1-*x4), (*y1-*y4) ) );
                p.drawText( r, Qt::AlignHCenter || Qt::AlignVCenter, *it++ );
                h = i + ticks_;
                for ( ; i<h; i+=subTicks_ )
                        p.drawLine( QPointF(*x1,*y1), QPointF(*x3,*y3) );
                i = h;
        }

        repaint();
}

// void Ruler::updateRuler()
// {
//         int sign = ( o_ == Qt::Horizontal ) ? 1 : -1;

//         QPainter p( &buffer_ );

//         if ( o_ == Qt::Vertical )
//                 p.rotate( 90 );

//         for ( double i = 0; 

void Ruler::setStart( int v )
{
        startPix_ = v;
        double sv = double(v);
        
        startTick_ = sv;// v % (int) floor(ticks_);
        startVal_ = 0;//floor( sv/ticks_ );

        calcTickMarks();
        updateRuler();
}

void Ruler::contextMenuEvent( QContextMenuEvent* )
{
}
