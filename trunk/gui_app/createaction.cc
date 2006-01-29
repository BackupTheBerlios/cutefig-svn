 
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

#include "createaction.h"
#include "addcommand.h"
#include "allobjects.h"

#include <QPainter>

void CreateAction::click( const QPoint& p, Fig::PointFlag f, const QMatrix* m )
{
        if ( !cObject_->pointSet( m->map( QPointF( p ) ), f ) ) {
                controler_->execAction( new AddCommand( selection_ ) );
                selection_.updateBackups();
        }
}


void CreateAction::move( const QPoint& p, const QMatrix* m )
{
        cObject_->cursorMove( m->map( QPointF( p ) ) );
}


void CreateAction::handleSelection()
{
        cObject_ = createObject();
        selection_.setObjectToBeCreated( cObject_ );
}

void CreateAction::drawMetaData( QPainter* p, const ViewBase* v ) const
{
        if ( cObject_ ) {
                v->drawObjectsPoints( p, cObject_ );
                p->setPen( Qt::red );
                v->drawObjectsMetaData( p, cObject_ );
        }
}

void CreateAction::reset()
{
        cObject_ = 0;
}

template<> void TCreateAction<Rectangle>::init()
{
        setShortcut( Qt::Key_R );
}

template<> void TCreateAction<Ellipse>::init()
{
        setShortcut( Qt::Key_E );
}

template<> void TCreateAction<Polygon>::init()
{
        setShortcut( Qt::Key_P );
}

template<> void TCreateAction<Polyline>::init()
{
        setShortcut( Qt::Key_L );
}

template<> void TCreateAction<BezierSpline>::init()
{
        setShortcut( Qt::Key_B );
}

template<> void TCreateAction<TextObject>::init()
{
        setShortcut( Qt::Key_T );
}
