 
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

#include "bezierspline.h"
#include "outputbackend.h"

#include <QPainterPath>

bool BezierSpline::pointHitsOutline( const QPointF& p, qreal tolerance ) const
{
        return false;
}

void BezierSpline::outputToBackend( OutputBackend *ob )
{
        ob->outputBezierSpline( this );
}

void BezierSpline::setupPainterPath()
{
        int s = points_.size()-1;
        int m = s % 3;
        s -= m;

        painterPath_ = QPainterPath();
        painterPath_.moveTo( points_[0] );
        
        for ( int i = 1; i < s; )
                painterPath_.cubicTo( points_[i++], points_[i++], points_[i++] );

        if ( m == 2 )
                painterPath_.quadTo( points_[s++], points_[s++] );
}
