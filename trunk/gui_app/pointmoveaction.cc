 
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

#include <QDebug>

bool PointMoveAction::wouldHandle( DrawObject* o, const QPoint& p, const QMatrix* m )
{
        if ( !m )
                return true;

        if ( !o )
                return false;

        return findPointUnderMouse( o, p, m );
}

bool PointMoveAction::wouldHandleSelection( const QPoint& p, const QMatrix* m )
{
        if ( pointToMove_ )
                return true;

        if ( selection_.objects().count() != 1 ) 
                return false;

        return wouldHandle( selection_.objects()[0], p, m );
}

QPointF* PointMoveAction::findPointUnderMouse( DrawObject* o, const QPoint& _p, const QMatrix* m )
//!++TODO++ the distance value 5 shoudn't be hardcoded
{
        if ( !o )
                return 0;

        QPointF *p, *f = 0;
        int i = 0;
        do {
                p = &o->points()[i++];
                if ( Geom::distance( m->inverted().map( *p ), _p ) < 5 )
                        f = p;
        } while ( !f && i<o->points().size() );

        return f;   
}

void PointMoveAction::reset()
{
        pointToMove_ = 0;
        wObject_ = 0;
}

void PointMoveAction::click( const QPoint& _p, Fig::PointFlag f, const QMatrix* m )
{
        if ( pointToMove_ && wObject_ ) {
                QPointF p = m->map( QPointF( _p ) );
                wObject_->pointSet( p, f );
                controler_->execAction( new ChangeCommand( selection_ ) );
                selection_.updateBackups();
                reset();
        } else {
                wObject_ = selection_.objects()[0];
                pointToMove_ = findPointUnderMouse( wObject_, _p, m );
                wObject_->setCurrentPoint( pointToMove_ );
        }
}

void PointMoveAction::move( const QPoint& p, const QMatrix* m )
{
        wObject_->cursorMove( m->map( QPointF( p ) ) );
}

