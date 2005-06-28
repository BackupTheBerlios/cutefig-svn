 
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
#include "compound.h"
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

void Figure::takeDrawObjects( const ObjectList& l )
{
        objectList_ = l;
        foreach ( DrawObject* o, l ) {
                addObjectToDrawingList( o );
                o->getReadyForDraw();
                o->doSpecificPreparation();
        }

        sortObjects();
}

/**
 *  \param o is the DrawObject that is sorted into the objectList_
 *  \param fin: if true the DrawObject is told to recalculate itself.
 */
void Figure::addDrawObject( DrawObject* o )
{
        objectList_.push_back( o );
//         if ( fin ) {
//                 o->getReadyForDraw();
//                 o->doSpecificPreparation();
//         }
         
        addObjectToDrawingList( o );
}

void Figure::addObjectToDrawingList( DrawObject* o )
{
        Compound* cpd = qobject_cast<Compound*>( o );
        if ( cpd )
                foreach ( DrawObject* co, cpd->childObjects() )
                        addObjectToDrawingList( co );
        else 
                sortIntoDrawingList( o );
}

/* code stolen from qalgorithms.h. I can't use the qUpperBound() as
 * containter contains pointers. The template would expand to
 * something like this:
 */
void Figure::sortIntoDrawingList( DrawObject* o )
{
        int n = drawingList_.size();
        int begin = 0;
        int half, middle;

        while ( n > 0 ) {
                half = n >> 1;
                middle = begin + half;
                if ( DrawObject::isLessThan( drawingList_[middle], o ) ) {
                        begin = middle + 1;
                        n -= half + 1;
                } else {
                        n = half;
                }
        }

        drawingList_.insert( begin, o );
}


void Figure::removeDrawObject( DrawObject* o )
{
        objectList_.removeAll( o );
        removeObjectFromDrawingList( o );
}

void Figure::removeObjectFromDrawingList( DrawObject* o )
{
        Compound* cpd = qobject_cast<Compound*>( o );
        if ( cpd )
                foreach( DrawObject* co, cpd->childObjects() )
                        removeObjectFromDrawingList( co );
        else
                drawingList_.removeAll( o );
}

void Figure::sortObjects()
{
        qSort( drawingList_.begin(), drawingList_.end(), DrawObject::isLessThan );
}

DrawObject* Figure::findContainingObject( const QPointF& p, qreal tolerance ) const
{
//         ObjectList::const_iterator it = objectList_.end();
//         while( it != objectList_.begin() ) 
//                 if ( (*--it)->pointHits( p, tolerance ) )
//                         return *it;

        foreach ( DrawObject* o, objectList_ ) 
                if ( o->pointHits( p, tolerance ) )
                        return o;
        
        return 0;            
}

void Figure::drawElements( QPainter* p, const ObjectList& backups ) const
{
        foreach ( DrawObject* o, drawingList_ )
                if ( !backups.contains( o ) )
                        o->draw( p );
}


void Figure::outputObjects( OutputBackend* ob ) const
{
        foreach ( DrawObject* o, objectList_ )
                o->outputToBackend( ob );
}


void Figure::clear()
{
        qDeleteAll( objectList_ );
        objectList_.clear();

        drawingList_.clear();
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
        foreach ( DrawObject* o, drawingList_ ) {
                int k = o->pen().dashesKey();
                if ( k >= 0 && !dkl.count( k ) )
                        dkl << k;
        }
        
        return dkl;
}
