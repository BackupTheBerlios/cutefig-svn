 
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

template<> void TCreateAction<TextObject>::init()
{
        setShortcut( Qt::Key_T );
}
