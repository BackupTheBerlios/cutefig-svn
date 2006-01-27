 
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

#include <cmath>

#include "drawobject.h"
#include "geometry.h"
#include "compound.h"

#include <QPainter>

#include <QDebug>


DrawObject::DrawObject( Figure* parent )
        : QObject( parent ),
          stroke_( Qt::black ),
          points_( 1 ),
          bRect_( 0,0,0,0 ),
          currentPointIndex_( 0 ),
          compoundParent_( 0 )
{
        figure_ = parent;
        depth_ = 50;
}

DrawObject::DrawObject( const DrawObject* o )
        : QObject( o->parent() ),
          pen_( o->pen_ ),
          stroke_( o->stroke_ ),
          fill_( o->fill_ ),
          depth_( o->depth_ ),
          commentString_( o->commentString_ ),
          points_( o->points_ ),
          currentPointIndex_( -1 ),
          compoundParent_( 0 )
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
        if ( currentPointIndex_ < 0 )
                return false;
        
        points_[currentPointIndex_] = pos;

        passPointFlag( f );

        currentPointIndex_ = nextPointIndex();
        if ( currentPointIndex_ < 0 ) { 
                doSpecificPreparation();
                return false;
        }
        
        return true;
}

void DrawObject::cursorMove( const QPointF & pos )
{
        if ( currentPointIndex_ < 0 )
                return;

        points_[currentPointIndex_] = pos;

        getReadyForDraw();
}


/*! The default implementation just uses #painterPath_ to draw it. So
 *  it does not have to be reimplemented by every subclass.
 */
void DrawObject::draw( QPainter* p ) const
{
        fill_.fillPath( painterPath_, p );
        pen_.strikePath( painterPath_, stroke_, p );
}

/*! This is useful to avoid that complex patterns have to be rendered
 *  while the object is moved with the mouse. The default
 *  implementation just uses #painterPath_ to draw it. So it does not
 *  have to be reimplemented by every subclass.
 *
 *  \e auxpen can be used if the object needs to draw some helpline
 *  while it is being created. See Ellipse::drawTentative for an
 *  example.
 */
void DrawObject::drawTentative( QPainter *p, const QPen& ) const
{
        p->drawPath( painterPath_ );
}

void DrawObject::mapMatrix( const QMatrix& m )
{
        points_ = m.map( points_ );
        doSpecificPreparation();
        getReadyForDraw();
}

/*! \e tolerance is a tolerance which is has to be variable as the
 *  zoom a figure is looked at might is variable.
 */
bool DrawObject::pointHits( const QPointF& p, qreal tolerance ) const
{
//         if ( !bRect_.intersects( Geom::centerRect( p, QSizeF( tolerance, tolerance ) ) ) )
//                 return false;
             
        if ( fill_ )
                return painterPath_.contains( p );
        else
                return pointHitsOutline( p, tolerance );
}

void DrawObject::getReadyForDraw()
{
        if ( points_.size() >= (int)minPoints() ) {
                setupPainterPath();
                setupRects();
        }
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

const ResourceSet DrawObject::usedResources() const
{
        ResourceSet rs;

        foreach ( AbstractResourceUser* aru, resourceUsers() )
                if ( aru )
                        rs[aru->resourceName()] << aru->key();
        
        return rs;
}


void DrawObject::releaseResources() const
{
        foreach ( AbstractResourceUser* aru, resourceUsers() )
                if ( aru )
                        aru->releaseResource();
}

void DrawObject::reclaimResources() const
{
        foreach ( AbstractResourceUser* aru, resourceUsers() )
                if ( aru )
                        aru->reclaimResource();
}

QList<AbstractResourceUser*> DrawObject::resourceUsers() const
{
        QList<AbstractResourceUser*> rul;

        rul << stroke_.resourceUser() << fill_.resourceUser() << pen_.resourceUser();
        addSpecificResourceUsers( rul );
        
        return rul;
}

const DrawObject* DrawObject::ancestor() const
{
        if ( compoundParent_ )
                return compoundParent_->ancestor(); 
        else
                return this;
}

void DrawObject::setCompoundParent( Compound* p )
{
        if ( !p && compoundParent_ )
                setParent( compoundParent_->parent() );
        else {
                compoundParent_ = p;
                setParent( p );
        }
}
