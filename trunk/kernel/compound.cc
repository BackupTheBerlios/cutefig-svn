 
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

/*! \class Compound
 *
 *  A compound has a list of DrawObjects called children. They are
 *  stored in childObjects_. Those DrawObjects are also the Compound's
 *  children in the meaning of QObject::children(). The DrawObjects
 *  are made the Compounds Qt-children when the compound is
 *  constructed. There is the function releaseChildren() that makes
 *  the children the children of the compounds parents. This is the
 *  current solution of the problem that the DrawObjects are to be
 *  deleted by Figure::clear().
 */

#include "compound.h"
#include "outputbackend.h"

#include <QDebug>

/*! Creates a Compound object of the DrawObjects in l. Those
 *  DrawObjects are made the children of the Compund.
 */
Compound::Compound( const ObjectList& l, Figure* parent )
        : DrawObject( parent ),
          childObjects_( l )
{
        foreach ( DrawObject* o, l ) 
                o->setCompoundParent( this );
}

Compound::Compound( const Compound* c )
        : DrawObject( c )
{
        foreach ( DrawObject* o, c->childObjects_ ) {
                DrawObject* newObject = o->copy();
                newObject->setCompoundParent( this );
                childObjects_.append( newObject );
        }
}

/*! Makes the children children of the Compounds parent. So the
 *  children are adopted by their grandparent so to speak. This parent
 *  can either the Figure or, if we deal with nested Compounds,
 *  another Compound.
 */
void Compound::releaseChildren()
{
        foreach ( DrawObject* o, childObjects_ )
                o->setCompoundParent( 0 );
}

void Compound::draw( QPainter* p ) const
{
        foreach ( DrawObject* o, childObjects_ )
                o->draw( p );
}

void Compound::drawTentative( QPainter* p ) const
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

void Compound::outputToBackend( OutputBackend* ob ) const
{
        ob->outputCompound( this );
}

void Compound::getReadyForDraw()
{
        bRect_ = QRectF();
        cRect_ = QRectF();
        foreach ( DrawObject* o, childObjects_ ) {
                o->getReadyForDraw();
                bRect_ |= o->boundingRect();
                cRect_ |= o->controlPointRect();
        }
}

void Compound::doSpecificPreparation()
{
        foreach ( DrawObject* o, childObjects_ )
                o->doSpecificPreparation();
}

void Compound::mapMatrix( const QMatrix& m )
{
        foreach ( DrawObject* o, childObjects_ ) 
                o->mapMatrix( m );
}

const ResourceSet Compound::usedResources() const
{
        ResourceSet rs;
        
        foreach ( DrawObject* o, childObjects_ ) {
                ResourceSet lrs = o->usedResources();
                
                for ( ResourceSet::const_iterator it = lrs.begin(); it != lrs.end(); ++it )
                        rs[it.key()] << it.value();
        }
        
        return rs;
}

void Compound::releaseResources()
{
        foreach ( DrawObject* o, childObjects_ ) 
                o->releaseResources();
}
        
void Compound::reclaimResources()
{
        foreach ( DrawObject* o, childObjects_ )
                o->reclaimResources();
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


