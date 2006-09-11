 
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

#include "pointmoveaction.h"
#include "drawobject.h"
#include "geometry.h"
#include "controler.h"

#include <QPainter>

#include <QDebug>


//! Finds the index of o->points() that is near the point _p and returns it or -1 if none is found.
int findPointUnderMouse( DrawObject* o, const QPoint& _p, const QMatrix& m )
{
        if ( !o )
                return -1;

        int i = 0;

        const QPolygonF& pts = o->points();
        while ( i < pts.size() && !Geom::isNear( m.inverted().map( pts[i] ), _p ) )
                ++i;
        
        return ( i == pts.size() ) ? -1 : i;   
}



PointMoveAction::PointMoveAction( Controler* parent )
        : InteractiveAction( parent ),
          pointIndex_( -1 )
{
        setText( tr("Move &Point") );
        setShortcut( Qt::CTRL+Qt::Key_P );
        cursor_ = Qt::SizeAllCursor;
}


bool PointMoveAction::wouldHandle( DrawObject* o, const QPoint& p, const QMatrix* m )
{
        if ( !m )
                return true;

        if ( !o )
                return false;
        
        return findPointUnderMouse( o, p, *m ) != -1;
}

bool PointMoveAction::wouldHandleSelection( const QPoint& p, const QMatrix* m )
{
        if ( pointIndex_ != -1 )
                return true;

        if ( selection_.objects().count() != 1 ) 
                return false;

        return wouldHandle( selection_.objects()[0], p, m );
}


void PointMoveAction::reset()
{
        pointIndex_ = -1;
        wObject_ = 0;
}

void PointMoveAction::click( const QPoint& _p, Fig::PointFlags f, const QMatrix& m )
{
        if ( pointIndex_ != -1 && wObject_ ) {
                QPointF p = m.map( QPointF( _p ) );
                wObject_->pointSet( p, f );
                controler_->execAction( new ChangeCommand( selection_ ) );
                selection_.updateBackups();
                reset();
        } else {
                wObject_ = selection_.objects()[0];
                pointIndex_ = findPointUnderMouse( wObject_, _p, m );
                wObject_->setCurrentPointIndex( pointIndex_ );
        }
}

void PointMoveAction::move( const QPoint& p, const QMatrix& m )
{
        wObject_->cursorMove( m.map( QPointF( p ) ) );
}


void PointMoveAction::drawMetaData( QPainter* p, const ViewBase* v ) const
{
        if ( selection_.objects().count() != 1 )
                return;

        const DrawObject* o = selection_.objects()[0];

        p->setPen( Qt::blue );
        
        v->drawObjectsPoints( p, o );
        v->drawObjectsMetaData( p, o );
}
