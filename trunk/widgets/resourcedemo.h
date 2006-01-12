 
/*****************************************************************************
**
**  $Id$
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

#ifndef resourcedemo_h
#define resourcedemo_h

#include <QFrame>
#include <QPainter>
#include <QPaintEvent>

#include "paintutils.h"

template<typename Resource> class ResourceDemo : public QFrame
{
public:
        ResourceDemo<Resource>( const Resource* r, QWidget* parent = 0 );
        ~ResourceDemo() {}

        void setResource( const Resource* r );

private:
        virtual void paintEvent( QPaintEvent* e );

        void doPainting( QPainter* p );

        const Resource* resource_;
};

template<typename Resource>
ResourceDemo<Resource>::ResourceDemo( const Resource* r, QWidget* parent )
        : QFrame( parent ),
          resource_( r )
{
        setMinimumSize( QSize( 90, 90 ) );
        setFrameStyle( Panel|Sunken );
        setLineWidth( 2 );
}

template<typename Resource>
void ResourceDemo<Resource>::setResource( const Resource* r )
{
        resource_ = r;
        update();
}

template<typename Resource>
void ResourceDemo<Resource>::paintEvent( QPaintEvent* e )
{
        QPainter pt( this );

        pt.setClipRegion( e->region() );
        pt.setRenderHint( QPainter::Antialiasing );

        PaintUtils::paintChessBoard( &pt, QRect( 0,0, width(), height() ) );

        doPainting( &pt );
        pt.end();

        QFrame::paintEvent( e );
}


#endif
