 
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



#include "brush.h"
#include <qpixmap.h>
#include <qcolor.h>
#include <stdio.h>

#include "brushdata.h"

void * Brush::patternArray_[22] = {
        &left30,
        &right30,
        &crosshatch30,
        &left45,
        &right45,
        &crosshatch45,
        &bricks,
        &vert_bricks,
        &horizontal,
        &vertical,
        &crosshatch,
        &leftshingle,
        &rightshingle,
        &vert_leftshingle,
        &vert_rightshingle,
        &fishscales,
        &small_fishscales,
        &circles,
        &hexagons,
        &octagons,
        &horiz_saw,
        &vert_saw };


Brush::Brush()
{
}

// QBrush Brush::qbrush( const int & pattern,
//                       const QColor & pencolor,
//                       const QColor & fillcolor )
// {
//         if ( pattern == -1 ) return QBrush::NoBrush;

//         char ** xpm;
//         QString col, bg;

//         if ( pattern > 40 ) {
//                 xpm = (char**)patternArray_[pattern-41];
//                 col = QString( "0 c " + fillcolor.name() );
//                 bg  = QString( "1 c " + pencolor.name() );
//         } else
//                 if ( pattern > 20 ) {
//                         xpm = (char**)shade_images[pattern-20];
//                         col = QString( "0 c " + fillcolor.name() );
//                         bg  = QString( "1 c #ffffff" );
//                 } else {
//                         xpm = (char**)shade_images[pattern];
//                         if ( fillcolor == QColor( "White" ) ) {
//                                 col = QString( "1 c #ffffff" );
//                                 bg  = QString( "0 c #000000" );
//                         } else if ( fillcolor == QColor( "Black" ) ) {
//                                 col = QString( "1 c #000000" );
//                                 bg  = QString( "0 c #ffffff" );
//                         } else
//                         {
//                                 bg  = QString( "0 c #000000" );
//                                 col = QString( "1 c " + fillcolor.name() );
//                         }
//                 }

//         xpm[1] = (char*)col.latin1();
//         xpm[2] = (char*)bg.latin1();
//         return QBrush( QColor( "White" ), QPixmap( (const char**) xpm ) );
// }
