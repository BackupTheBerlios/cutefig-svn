 
/*****************************************************************************
**
**  @version $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2005 Johannes Mueller, joh@users.berlios.net
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


#include "lineshowwidget.h"

#include <QPainter>
#include <QPainterPath>
#include <QPolygonF>
#include <QColor>
#include <QSize>

LineShowWidget::LineShowWidget( const Pen& pen,
                                const int h, 
                                QWidget * parent )
        : QWidget( parent ),
          pen_( pen )
{
        setFixedHeight( h );
        update();
}

void LineShowWidget::setColor( QColor c )
{
        pen_.setColor( c );
        myUpdate();
}

void LineShowWidget::setWidth( double w )
{
        pen_.setWidth( w );
        myUpdate();
}

void LineShowWidget::setCapStyle( int cs )
{
        pen_.setCapStyle( (Qt::PenCapStyle) cs );
        myUpdate();
}

void LineShowWidget::setJoinStyle( int js )
{
        pen_.setJoinStyle( (Qt::PenJoinStyle) js );
        myUpdate();
}

void LineShowWidget::paintEvent( QPaintEvent * )
{
        QPainter p( this );

        int w = width();
        int h = height();
        p.drawRect( 0,0, w,h );
        p.fillRect( 0,0, w,h, Qt::white );

        QPolygonF pa(10);
        for( int i=0; i<10; i++ ) 
                pa[i] = QPointF( w/10*(1+i), i%2 ? h*1/3 : h*2/3 );


        p.setRenderHint( QPainter::Antialiasing, true );
        QPainterPath path;
        path.addPolygon( pa );
        pen_.drawPath( path, &p );

        p.end();
}

void LineShowWidget::setDashes( int d )
{
        pen_.setDashes( d );
        myUpdate();
}

void LineShowWidget::myUpdate()
{
        emit( changed( pen_ ) );
        update();
}
