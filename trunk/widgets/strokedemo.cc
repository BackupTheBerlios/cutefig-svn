 
/*****************************************************************************
**
**  $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2005 Johannes Mueller, johmue@users.sourceforge.net
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

#include "strokedemo.h"
#include "stroke.h"
#include "pen.h"

#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>

#include <QDebug>

StrokeDemo::StrokeDemo( const Stroke& stroke, QWidget* parent )
        : QFrame( parent ),
          stroke_( stroke )
{
        setMinimumSize( QSize( 90, 90 ) );
        setFrameStyle( Panel|Sunken );
        setLineWidth( 2 );
}

void StrokeDemo::setStroke( const Stroke& stroke )
{
        stroke_ = stroke;
        repaint();
}

void StrokeDemo::paintEvent( QPaintEvent* e )
{
        QPainter pt( this );
        pt.setClipRegion( e->region() );
        pt.setRenderHint( QPainter::Antialiasing );
        int w = width();
        int h = height();
        int w3 = w/3;
        int h3 = h/3;
        int w9 = w3/3;
        int h9 = h3/3;

        qDebug() << w << w3 << w9;
        qDebug() << h << h3 << h9;
        
        pt.fillRect( 0,0, w,h, Qt::white );        

        QPainterPath el1;
        el1.addEllipse( w9,h9, w3,h3 );

        Pen pen;
        pen.setWidth( 3 );
        pen.strikePath( el1, stroke_, &pt );

        QPainterPath el2;
        el2.addEllipse( w9,5*h9, w3,h3 );
        stroke_.fillPath( el2, &pt );

        QFrame::paintEvent( e );
}
