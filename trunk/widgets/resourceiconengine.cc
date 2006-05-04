 
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

#include "resourceiconengine.h"
#include "typedefs.h"
#include "gradient.h"

#include <QPainter>

#include <QDebug>

void AbstractResourceIconEngine::paint( QPainter* p, const QRect& r,
                                        QIcon::Mode mode, QIcon::State )
{
        int w = r.width();
        int h = r.height();
        int w2 = w >> 1;
        int h2 = h >> 1;

        p->fillRect(  0, 0, w2,h2, Qt::lightGray );
        p->fillRect( w2,h2, w2,h2, Qt::lightGray );
        p->fillRect(  0,h2, w2,h2, Qt::darkGray );
        p->fillRect( w2, 0, w2,h2, Qt::darkGray );

        if ( mode == QIcon::Disabled ) {
                p->setPen( Qt::red );
                int x1 = w2 >> 1;
                int y1 = h2 >> 1;
                int x2 = w - x1;
                int y2 = h - y1;
                
                p->drawLine( x1,y1, x2,y2 );
                p->drawLine( x1,y2, x2,y1 );
        }
        else
                paintForeground( p, r );
}

template<>
void ResourceIconEngine<QColor>::paintForeground( QPainter* p, const QRect& r )
{
        p->fillRect( r, resource_ );
}

template<>
void ResourceIconEngine<Gradient>::paintForeground( QPainter* p, const QRect& r )
{
        QGradient* qg = resource_.toQGradient( r );
        if ( qg )
                p->fillRect( r, *qg );
}

template<>
void ResourceIconEngine<QPixmap>::paintForeground( QPainter* p, const QRect& r )
{
        p->fillRect( r, resource_ );
}

template<>
void ResourceIconEngine<Dashes>::paintForeground( QPainter* p, const QRect& r )
{
        p->fillRect( r, Qt::white );

        QPen pen;
        pen.setWidth( 2 );
        pen.setDashPattern( resource_ );        
        p->setPen( pen );

        int h2 = r.height()/2;
        
        p->drawLine( 0,h2, r.width(), h2 );
}

void ResourceIconEngine<WhiteIcon>::paintForeground( QPainter* p, const QRect& r )
{
        p->fillRect( r, Qt::white );
}


#include "arrow.h"

template<>
void ResourceIconEngine<ArrowPainter*>::paintForeground( QPainter* p, const QRect& r )
{
        Arrow a( resource_ );

//         double mw = r.width() * .8;
//         double mh = r.height() * .8;
        
//         if ( a.length() > mw ) {
//                 double f = a.length() / mw;
//                 a.setLength( mw );
//                 a.setWidth( a.width() / f );
//         }

//         if ( a.width() > mh ){
//                 double f = a.width() / mh;
//                 a.setWidth( mh );
//                 a.setLength( a.length() / f );
//         }

        QPen pen;
        pen.setWidth( 2 );
        
        p->setPen( pen );
        p->setRenderHint( QPainter::Antialiasing );
        p->fillRect( r, Qt::white );
        
        QPointF pos( r.width() * .1, r.height() / 2 );
        a.draw( pos, QPointF(-1,0), p );
        p->drawLine( pos, QPointF( r.width() *.9, r.height() / 2 ) ); 
}

