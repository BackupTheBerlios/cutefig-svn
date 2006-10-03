 
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

#include "pixoutput.h"
#include "figure.h"

#include <QImage>
#include <QPainter>
#include <QImageWriter>
#include <QFile>
#include <QPaintEngine>

#include <QDebug>

PIXOutput::PIXOutput( QByteArray format )
        : ExportFilter(),
          format_( format ),
          xres_( 0 ),
          yres_( 0 ),
          scale_( 1.0 ),
          backgroundColor_(),
          quality_( 100 ),
          gamma_( 1.0 ),
	  withPaper_( false )
{
}

PIXOutput::PIXOutput( const Figure* figure, QFile* file, QByteArray format )
                : ExportFilter( figure, file ),
                  format_( format ),
                  xres_( 0 ),
                  yres_( 0 ),
                  scale_( 1.0 ),
                  backgroundColor_(),
                  quality_( 100 ),
                  gamma_( 1.0 ),
		  withPaper_( false )
{
}

void PIXOutput::exportFigure()
{
        if ( figure_->isEmpty() ) {
//                errorString_ = tr("Figure is empty. Nothing exported.");
                return;
        }
        
        QSize sz;
	if ( withPaper_ )
		sz = figure_->paper().size().toSize();
	else
		sz = figure_->boundingRect().size().toSize();
	
        QSize nsz = sz;

        double scaleX = scale_;
        double scaleY = scale_;
        nsz.setWidth( qRound( sz.width() * scaleX ) );
        nsz.setHeight( qRound( sz.height() * scaleY ) );        

        if ( xres_ ) {
                int y = qRound( double(sz.height())/sz.width() * xres_ );
                nsz = QSize( xres_, y );
        }

        if ( yres_ ) {
                if ( !xres_ ) {
                        int x = qRound( double(sz.width())/sz.height() * yres_ );
                        nsz.setWidth( x );
                }
                nsz.setHeight( yres_ );
        }

        QImage image( nsz, QImage::Format_ARGB32_Premultiplied );

        QPainter painter( &image );
        painter.setBackgroundMode( Qt::OpaqueMode );
        painter.setRenderHint( QPainter::Antialiasing );
        painter.setRenderHint( QPainter::TextAntialiasing );
        painter.setRenderHint( QPainter::SmoothPixmapTransform );

        if ( !backgroundColor_.isValid() ) {
                if ( format_ == "jpg" || format_ == "jpeg" || format_ == "bmp" )
                        painter.fillRect( 0,0, nsz.width(), nsz.height(), Qt::white );
        }
        else
                painter.fillRect( 0,0, nsz.width(),nsz.height(), backgroundColor_ );
        
        painter.scale( double(nsz.width())/sz.width(), double(nsz.height())/sz.height() );

	if ( !withPaper_ )
		painter.translate( - figure_->boundingRect().topLeft() );

        figure_->drawElements( &painter );

        painter.end();

        QImageWriter writer( file_, format_ );
        writer.setQuality( quality_ );
        writer.setGamma( gamma_ );

        error_ = !writer.write( image );
        if ( error_ )
                errorString_ = writer.errorString();
        else
                errorString_ = QString();
}

void PIXOutput::setScale( double s )
{
        if ( s > 0 )
                scale_ = s;
}

void PIXOutput::setXres( int x )
{
        if ( x > 0 )
                xres_ = x;
}

void PIXOutput::setYres( int y )
{
        if ( y > 0 )
                yres_ = y;
}
