 
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

/** \class DrawObject
 *
 *  It contains all the basic data of a DrawObject like pen_,
 *  commentString_... 
 *
 *  Many functions are virtual or even pure virtual as it is the
 *  subclasses job to implement them.
 */

#include <cmath>

#include "drawobject.h"
#include "brush.h"
#include "geometry.h"

#include <QPainter>

#include <QDebug>

unsigned DrawObject::clickTolerance_ = 10;

DrawObject::DrawObject( Figure* parent )
        : QObject( parent ),
          points_( 1 ),
          currentPoint_( 0 ),
          bRect_( 0,0,0,0 )
{
        figure_ = parent;
        depth_ = 50;
}

DrawObject::DrawObject( DrawObject* o )
        : QObject( o->parent() ),
          pen_( o->pen_ ),
          fillStroke_( o->fillStroke_ ),
          depth_( o->depth_ ),
          commentString_( o->commentString_ ),
          points_( o->points_ )
{
}


void DrawObject::move( const QPointF& d )
{
        points_.translate( d.x(),d.y() );
        getReadyForDraw();
        doSpecificPreparation();
}

bool DrawObject::pointSet( const QPointF & pos, Fig::PointFlag f )
{
        if ( currentPoint_ ) 
                *currentPoint_ = pos;             
        else 
                points_[0] = pos;

        passPointFlag( f );

        QPointF* np = nextPoint();
        if ( !np ) 
                doSpecificPreparation();

        currentPoint_ = np;
        
        return np;
}

void DrawObject::cursorMove( const QPointF & pos )
{
        if ( !currentPoint_ ) 
                return;

        *currentPoint_ = pos;

        getReadyForDraw();
}

void DrawObject::draw( QPainter* p )
{
        fillStroke_.fillPath( painterPath_, p );
        pen_.drawPath( painterPath_, p );
}

void DrawObject::drawTentative( QPainter *p )
{
        p->drawPath( painterPath_ );
}

void DrawObject::mapMatrix( const QMatrix& m )
{
        points_ = m.map( points_ );
        doSpecificPreparation();
        getReadyForDraw();
}

bool DrawObject::pointHits( const QPointF& p, qreal tolerance ) const
{
//         if ( !bRect_.intersects( Geom::centerRect( p, QSizeF( tolerance, tolerance ) ) ) )
//                 return false;
             
        if ( fillStroke_ )
                return painterPath_.contains( p );
        else
                return pointHitsOutline( p, tolerance );
}

void DrawObject::getReadyForDraw()
{
        setupPainterPath();
        setupRects();
}

void DrawObject::setupRects()
{
        cRect_ = painterPath_.controlPointRect();
        bRect_ = pen_.pathRect( painterPath_ );
}

void DrawObject::setPen( const Pen& p )
{
        pen_ = p;
        setupRects();
}
