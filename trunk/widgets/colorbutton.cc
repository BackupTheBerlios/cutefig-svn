 
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


#include "colorbutton.h"

#include <QColor>
#include <QColorDialog>
#include <QPainter>

ColorButton::ColorButton( QWidget * parent )
        : QPushButton( parent ),
          pixmap_( iconSize() - QSize( 5,5 ) )
{
        ColorButton( Qt::blue, parent );
}

ColorButton::ColorButton( const QColor &c, 
                          QWidget * parent )
        : QPushButton( parent ),
          pixmap_( iconSize() )// - QSize( 5,5 ) )
{
        setColor( c );
        connect( this, SIGNAL( clicked() ), this, SLOT( changeColor() ) );
}

void ColorButton::setColor( const QColor &c )
{
        color_ = c;
        draw();
        emit( colorChanged( c ) );
}


void ColorButton::changeColor()
{
        QRgb rgb = QColorDialog::getRgba( color_.rgba() );

        color_ = QColor( rgb );
        color_.setAlpha( qAlpha( rgb ) );
        setColor( color_ );
        emit colorChanged( color_ );
}

void ColorButton::draw()
{
        QPainter p( &pixmap_ );
        p.setPen( Qt::SolidLine );

        int w = iconSize().width();
        int h = iconSize().height();
        int w2 = w / 2;
        int h2 = h / 2;
        
        p.fillRect( 0,0, w2,h2, Qt::darkGray );
        p.fillRect( w2,h2, w,h, Qt::darkGray );
        p.fillRect( w2,0, w,h2, Qt::lightGray );
        p.fillRect( 0,h2, w2,h, Qt::lightGray );
        
        p.setBrush( color_ ); 
        p.drawRect( 0,0, pixmap_.size().width(), pixmap_.size().height() );
        setIcon( pixmap_ );
}
