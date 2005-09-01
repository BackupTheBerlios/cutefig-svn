 
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
#include "stroke.h"

#include <QPixmap>
#include <QPainter>
#include <QPainterPath>

#include <QDebug>

StyleComboBox::StyleComboBox( QWidget * parent )
        : QComboBox( parent ),
          keyMap_()
{
        addItem( QIcon( drawItemQtPen(Qt::NoPen) ), QString() );
        keyMap_ << ResourceKey();
        
        DashesLib& dl = DashesLib::instance();

        foreach ( ResourceKey key, dl.keys() ) {
                addItem( drawItemCustomPen( key ), QString() );
                keyMap_ << key;
        }

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


QPixmap StyleComboBox::drawItemCustomPen( const ResourceKey& dkey )
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

        p.setRenderHint( QPainter::Antialiasing, false );
        QPainterPath path( QPoint( w/10, h/2 ) );
        path.lineTo( QPoint(w-w/10,h/2) );
        pen.strikePath( path, Stroke(Qt::black), &p );

        p.end();
    
        return pm;
}

void StyleComboBox::em_activated( int i )
{
        emit activatedDash( keyMap_[i] );
}

void StyleComboBox::em_highlighted( int i )
{
        emit highlightedDash( keyMap_[i] );
}

void StyleComboBox::setCurrentIndex( const ResourceKey& key )
{
        QComboBox::setCurrentIndex( keyMap_.indexOf( key ) );
}
