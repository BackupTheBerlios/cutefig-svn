 
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

#include "compound.h"

Compound::Compound( const ObjectList& l, Figure* parent )
        : DrawObject( parent ),
          childObjects_( l )
{
        qSort( childObjects_.begin(), childObjects_.end(), DrawObject::isLessThan );
}

Compound::Compound( Compound* c )
        : DrawObject( c )
{
        foreach ( DrawObject* o, c->childObjects_ )
                childObjects_.append( o->copy() );

        getReadyForDraw();
        doSpecificPreparation();
}

void Compound::draw( QPainter* p )
{
        foreach ( DrawObject* o, childObjects_ )
                o->draw( p );
}

void Compound::drawTentative( QPainter* p )
{
        foreach ( DrawObject* o, childObjects_ )
                o->drawTentative( p );
}

void Compound::move( const QPointF& d )
{
        foreach ( DrawObject* o, childObjects_ )
                o->move( d );
        getReadyForDraw();
}

void Compound::outputToBackend( OutputBackend* )
{
        
}

void Compound::getReadyForDraw()
{
        bRect_ = QRectF();
        cRect_ = QRectF();
        foreach ( DrawObject* o, childObjects_ ) {
                o->getReadyForDraw();
                bRect_ |= o->boundingRect();
                cRect_ != o->controlPointRect();
        }
}

void Compound::doSpecificPreparation()
{
        foreach ( DrawObject* o, childObjects_ )
                o->doSpecificPreparation();
}

void Compound::mapMatrix( const QMatrix& m )
{
        foreach ( DrawObject* o, childObjects_ ) {
                o->mapMatrix( m );
        }
        doSpecificPreparation();
}

QPointF Compound::center() const
{
        return bRect_.center();
}

bool Compound::pointHits( const QPointF& p, qreal tolerance ) const
{
        foreach ( DrawObject* o, childObjects_ )
                if ( o->pointHits( p, tolerance ) )
                        return true;

        return false;
}

bool Compound::pointHitsOutline( const QPointF& p, qreal tolerance ) const
{
        foreach ( DrawObject* o, childObjects_ )
                if ( o->pointHitsOutline( p, tolerance ) )
                        return true;

        return false;    
}

