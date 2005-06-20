 
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


#include "stylecombobox.h"

#include <QPixmap>
#include <QPainter>
#include <QPainterPath>

#include <QDebug>

StyleComboBox::StyleComboBox( QWidget * parent )
        : QComboBox( parent )
{
        for ( int i = 0; i < 6; ++i ) 
                insertItem( drawItemQtPen( Qt::PenStyle( i ) ) );

        DashesLib& dl = DashesLib::instance();
        for ( int i = 0; i < dl.size(); ++i )
                insertItem( drawItemCustomPen( i ) );

        connect( this, SIGNAL( activated( int ) ), 
                 this, SLOT( em_activated( int ) ) );
        connect( this, SIGNAL( highlighted( int ) ), 
                 this, SLOT( em_highlighted( int ) ) );
}


QPixmap StyleComboBox::drawItemQtPen( Qt::PenStyle ps )
{
        int w = size().width();
        int h = size().height()/2;

        QPixmap pm( QSize( w,h ) );
    
        QPainter p;
        p.begin( &pm );
        p.fillRect( 0,0, w,h, Qt::white );
        QPen pen( Qt::black, 1, ps );

        p.setPen( pen );
        p.setRenderHint( QPainter::Antialiasing, false );
        p.drawLine( w/10, h/2, w-w/10,h/2 );
        p.end();
    
        return pm;
}


QPixmap StyleComboBox::drawItemCustomPen( int dkey )
{
        int w = size().width();
        int h = size().height()/2;

        QPixmap pm( QSize( w,h ) );
    
        QPainter p;
        p.begin( &pm );
        p.fillRect( 0,0, w,h, Qt::white );
        Pen pen;
        pen.setDashes( dkey );
        pen.setWidth( 1.0 );
        pen.setColor( Qt::black );

        p.setRenderHint( QPainter::Antialiasing, false );
        QPainterPath path( QPoint( w/10, h/2 ) );
        path.lineTo( QPoint(w-w/10,h/2) );
        pen.drawPath( path, &p );

        p.end();
    
        return pm;
}

void StyleComboBox::em_activated( int i )
{
        emit activatedDash( i - 6 );
}

void StyleComboBox::em_highlighted( int i )
{
        emit highlightedDash( i - 6 );
}

void StyleComboBox::setCurrentIndex( int index )
{
        QComboBox::setCurrentIndex( index + 6 );
}
