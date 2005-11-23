 
/*****************************************************************************
**
**  $Id: paintutils.h 69 2005-11-20 23:19:56Z joh $
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


#include "paintutils.h"

#include <QPainter>

#include <cmath>

void PaintUtils::paintChessBoard( QPainter* painter, const QRect& rect, int fw )
{
        QRect r;
        r.setSize( QSize( fw,fw ) );

        int w = rect.width();
        int h = rect.height();
        
        bool inc = !( int(ceil(double(h)/fw))%2 );
        
        int i = 0;
        for ( int x = 0; x < w; x += fw ) {
                for( int y = 0; y < h; y += fw ) {
                        r.moveTopLeft( QPoint( x,y ) );
                        QColor c( ( i++ % 2 ) ? Qt::lightGray : Qt::darkGray );
                        
                        painter->fillRect( r, c );
                }
                if ( inc )
                        ++i;
        }
}
