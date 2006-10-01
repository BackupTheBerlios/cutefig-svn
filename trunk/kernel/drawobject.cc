
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


#include "drawobject.h"
#include "geometry.h"
#include "compound.h"

#include <QPainter>

#include <cmath>

#include <QDebug>


DrawObject::DrawObject( Figure* parent )
        : QObject( parent ),
          figure_( parent ),
          stroke_( Qt::black ),
          depth_( 50 ),
          points_( 1 ),
          bRect_( 0,0,0,0 ),
          currentPointIndex_( 0 ),
          compoundParent_( 0 ),
          startArrow_(),
          endArrow_()
{
}

DrawObject::DrawObject( const DrawObject* o )
        : QObject( o->parent() ),
          figure_( o->figure_ ),
          pen_( o->pen_ ),
          stroke_( o->stroke_ ),
          fill_( o->fill_ ),
          depth_( o->depth_ ),
          commentString_( o->commentString_ ),
          points_( o->points_ ),
          currentPointIndex_( -1 ),
          compoundParent_( 0 ),
          startArrow_( o->startArrow_ ),
          endArrow_( o->endArrow_ )
{
}

void DrawObject::move( const QPointF& d )
{
        points_.translate( d.x(),d.y() );
        getReadyForDraw();
        doSpecificPreparation();
}

bool DrawObject::pointSet( const QPointF & pos, Fig::PointFlags f )
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
//        qDebug() << "draw" << this;
        fill_.fillPath( painterPath_, p );
        pen_.strikePath( painterPath_, stroke_, p );

//        if ( startArrow_ || endArrow_ )
                drawArrows( p );
}

void DrawObject::drawArrows( QPainter* p ) const
{
        if ( painterPath_.isEmpty() )
                return;
        
        p->setPen( QPen( stroke_.brush( bRect_ ), pen_.width() ) );
        p->setBrush( fill_.brush( bRect_ ) );
        
        QList<QPolygonF> pols = painterPath_.toSubpathPolygons();

        const QPolygonF& first = pols.first();
        const QPolygonF& last = pols.last();
        

        if ( startArrow_.isValid() )
                startArrow_.draw( first.first(), startAngle( first ), p );
        if ( endArrow_.isValid() )
                endArrow_.draw( last.last(), endAngle( last ), p );
}


QPointF DrawObject::startAngle( const QPolygonF& pol ) const
{
        if ( pol.isEmpty() )
                return QPointF();

        QPointF d = pol[0] - pol[1];
        return d / hypot( d.x(), d.y() );
}


QPointF DrawObject::endAngle( const QPolygonF& pol ) const
{
        if ( pol.isEmpty() )
                return QPointF();

        int i = pol.size() -1;
        QPointF d = pol[i] - pol[i-1];
        return d / hypot( d.x(), d.y() );
}

/*! This is useful to avoid that complex patterns have to be rendered
 *  while the object is moved with the mouse. The default
 *  implementation just uses #painterPath_ to draw it. So it does not
 *  have to be reimplemented by every subclass.
 */
void DrawObject::drawTentative( QPainter *p ) const
{
//        qDebug() << this;// << painterPath_;
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
        if ( fill_ )
                return painterPath_.contains( p );
        else
                return pointHitsOutline( p, tolerance );
}

bool DrawObject::pointHitsOutline( const QPointF& p, qreal tolerance ) const
{
        QRectF r( Geom::centerRect( p, QSizeF(tolerance, tolerance) ) );

        return !painterPath_.contains( r ) && painterPath_.intersects( r );
}


void DrawObject::getReadyForDraw()
{
        if ( points_.size() >= minPoints() ) {
                setupPainterPath();
                setupRects();
        }
}

void DrawObject::setupRects()
{
        cRect_ = points_.boundingRect();
        bRect_ = pen_.pathRect( painterPath_ );
}

void DrawObject::setPen( const Pen& p )
{
        pen_ = p;
//        setupRects();
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



QString DrawObject::objectname_stripped() const
{
        QString n = objectname();
        return n.remove('&');
}
