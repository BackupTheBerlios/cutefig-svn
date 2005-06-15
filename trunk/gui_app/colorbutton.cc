 
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


#include "colorbutton.h"

//Added by the Qt porting tool:
#include <QResizeEvent>

#include <qcolor.h>
#include <qcolordialog.h>
#include <qpixmap.h>
#include <qpainter.h>

ColorButton::ColorButton( QWidget * parent )
        : QPushButton( parent ),
          pixmap_( size() - QSize( 5,5 ) )
{
        ColorButton( Qt::blue, parent );
}

ColorButton::ColorButton( const QColor &c, 
                          QWidget * parent )
        : QPushButton( parent ),
          pixmap_( size() - QSize( 5,5 ) )
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
        p.setBrush( color_ ); 
        p.drawRect( 0,0, pixmap_.size().width(), pixmap_.size().height() );
        setIcon( pixmap_ );
}

void ColorButton::resizeEvent( QResizeEvent* )
{
//    pixmap_.resize( size() - QSize( 5,0 ) );
//    draw();
}
