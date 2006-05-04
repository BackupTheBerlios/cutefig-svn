 
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

#include "resourcedemo.h"
#include "gradient.h"

template<>
void ResourceDemo<QColor>::doPainting( QPainter* p )
{
        int w3 = width()/3;
        int h3 = height()/3;
        
        p->fillRect( w3,h3, w3,h3, *resource_ );
}

template<>
void ResourceDemo<Gradient>::doPainting( QPainter* p )
{
        int w = width();
        int h = height();
        int w3 = w/3;
        int h3 = h/3;
        int w9 = w3/3;
        int h9 = h3/3;
        w3 *=2;

        QGradient* qGrad = resource_->toQGradient( QRect( w9,h9, w3,h3 ) );

        if ( !qGrad )
                return;

        p->setPen( QPen( *qGrad, 3.0 ) );
        p->drawEllipse( w9,h9, w3,h3 );

        h9 *= 5;

        delete qGrad;

        qGrad = resource_->toQGradient( QRect( w9,h9, w3,h3 ) );
        p->setPen( Qt::NoPen );
        p->setBrush( *qGrad ); 
        p->drawEllipse( w9,h9, w3,h3 );

        delete qGrad;
}


template<>
void ResourceDemo<QPixmap>::doPainting( QPainter* p )
{
        int w = width();
        int h = height();
        int w3 = w/3;
        int h3 = h/3;
        int w9 = w3/3;
        int h9 = h3/3;
        w3 *=2;

        p->setPen( QPen( QBrush( *resource_ ), 3.0 ) );
        p->drawEllipse( w9,h9, w3,h3 );

        h9 *= 5;

        p->setPen( Qt::NoPen );
        p->setBrush( *resource_ ); 
        p->drawEllipse( w9,h9, w3,h3 );
}

