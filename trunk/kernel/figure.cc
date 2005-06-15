 
/*****************************************************************************
**
**  @version $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2005 Johannes Mueller, joh@users.berlios.net
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

/** \class Figure
 *
 *  The Figure is the Model in the MVC approach. It takes a pointer to
 *  a Controler by setControler() which is usually called by an
 *  instance of a Controler. The Controler tells the Figure about
 *  updates the user made. This is done by using the routines
 *  addDrawObject() and removeDrawObject().
 *
 *  To paint the figure on a view, the Figure can be passed a pointer
 *  to a QPainter by the routine drawElements(). All elements will
 *  be drawn to this painter.
 */

#include "figure.h"
#include "drawobject.h"
#include "geometry.h" 

#include <QPainter>
#include <QPointF>
#include <QRectF>
#include <QtAlgorithms>

#include <QDebug>

Figure::Figure( QObject *parent ) :
        QObject( parent ),
        scale_( 1.0 )
{
}

/**
 *  \param o is the DrawObject that is sorted into the objectList_
 *  \param fin: if true the DrawObject is told to recalculate itself.
 */
void Figure::addDrawObject( DrawObject* o, bool fin )
//!++TODO++ find a more efficient algorithm to sort the object in.  
{
        if ( !objectList_.empty() ) {
                ObjectList::iterator it = objectList_.begin();
        
                while( it != objectList_.end() && o->depth() < (*it)->depth() )
                        ++it;

                objectList_.insert( it, o );
        } else 
                objectList_.push_back( o );

        if ( fin ) {
                o->getReadyForDraw();
//                o->calculateRegion();
                o->doSpecificPreparation();
        }
}

void Figure::removeDrawObject( DrawObject* o )
{
        objectList_.removeAll( o );
}

void Figure::sortObjects()
{
        qSort( objectList_.begin(), objectList_.end(), DrawObject::isLessThan );
}

DrawObject* Figure::findContainingObject( const QPointF& p, qreal tolerance ) const
{
        ObjectList::const_iterator it = objectList_.end();
        while( it != objectList_.begin() ) 
                if ( (*--it)->pointHits( p, tolerance ) )
                        return *it;

        return 0;            
}

void Figure::drawElements( QPainter* p, const ObjectList& backups ) const
{
        foreach ( DrawObject* o, objectList_ )
                if ( !backups.contains( o ) )
                        o->draw( p );
}


void Figure::outputObjects( OutputBackend* ob ) const
{
        ObjectList::const_iterator it = objectList_.begin();
        for( ; it != objectList_.end(); ++it )
                        (*it)->outputToBackend( ob );
}


void Figure::clear()
{
        ObjectList::iterator it = objectList_.begin();
        for ( ; it != objectList_.end(); ++it )
                delete *it;
        
        objectList_.erase( objectList_.begin(), objectList_.end() );        
}


QRectF Figure::boundingRect() const
{
        QRectF r;
        foreach( DrawObject* o, objectList_ )
                r |= o->boundingRect();
        return r;
}

const DashKeyList Figure::dashList() const
{
        DashKeyList dkl;
        foreach ( DrawObject* o, objectList_ ) {
                int k = o->pen().dashesKey();
                if ( k >= 0 && !dkl.count( k ) )
                        dkl << k;
        }
        
        return dkl;
}
