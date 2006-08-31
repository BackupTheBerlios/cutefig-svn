 
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

#include "relativeaction.h"
#include "drawobject.h"
#include "changecommand.h"
#include "controler.h"

#include <QDebug>

void RelativeAction::click( const QPoint& _p, Fig::PointFlags f, const QMatrix* m )
{
        QPointF p = m->map( QPointF( _p ) );
        if ( last_.isNull() ) {
                last_ = p;
                return;
        }
        
        relativeClick( p - last_, f );

        if ( finished_ ) {
                controler_->execAction( new ChangeCommand( selection_ ) );
                selection_.updateBackups();
        }
}

void RelativeAction::move( const QPoint& _p, const QMatrix* m )
{
        QPointF p = m->map( QPointF( _p ) );
        relativeMove( p - last_ );
        last_ = p;
}


bool RelativeAction::wouldHandle( DrawObject* o, const QPoint& p, const QMatrix* m )
//!< ++TODO++ get rid of 5*m->m11()
{
        if ( !m || !last_.isNull() )
                return true;

        if ( !o )
                return false;

//        return o->region().contains( m->map( p ) );
        return o->pointHits( m->map( p ), 5*m->m11() );
}


void RelativeAction::reset()
{
        finished_ = true;
        last_ = QPoint();
}

void RelativeAction::relativeClick( const QPointF& p, Fig::PointFlags f )
{
        foreach ( DrawObject* o, selection_.objects() )
                relativeClick( o, p, f );
}

void RelativeAction::relativeMove( const QPointF& p )
{
        foreach ( DrawObject* o, selection_.objects() )
                relativeMove( o, p );
}
