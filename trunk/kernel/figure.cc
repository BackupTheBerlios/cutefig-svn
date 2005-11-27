 
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

/** Then tells them to recalculate. Usually called by a Parser.
 */
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

/** Takes a single DrawObject. It is assumed that the DrawObject
 *  already calculated itself. First the Resources used by the
 *  DrawObject are reclaimed.
 */
void Figure::addDrawObject( DrawObject* o )
{
        o->reclaimResources();
        objectList_.push_back( o );
        addObjectToDrawingList( o );
}

/** Adds a DrawObject to the drawingList_. If o is a Compound its
 *  childObjects are inserted recursively. The Compound itself is not
 *  inserted.
 */
void Figure::addObjectToDrawingList( DrawObject* o )
{
        Compound* cpd = qobject_cast<Compound*>( o );
        if ( cpd )
                foreach ( DrawObject* co, cpd->childObjects() )
                        addObjectToDrawingList( co );
        else 
                sortIntoDrawingList( o );
}

/** Sorts the DrawObject o into the drawingList_ according to it's
 *  depth. The code is stolen from qalgorithms.h. I can't use the
 *  qUpperBound() as containter contains pointers. The template would
 *  expand to something like this:
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
                } else 
                        n = half;
        }

        drawingList_.insert( begin, o );
}

/** First releases the Resource used by the DrawObject. Then removes a
 *  DrawObject from the objectList_ and from the drawingList.
 */
void Figure::removeDrawObject( DrawObject* o )
{
        o->releaseResources();
        objectList_.removeAll( o );
        removeObjectFromDrawingList( o );
}

/** Removes a DrawObject from the drawingList_. If it is a Compound
 *  all its children are removed. The Compound itself does not have to
 *  be removed as drawingList_ does not contain the Compounds.
 */
void Figure::removeObjectFromDrawingList( DrawObject* o )
{
        Compound* cpd = qobject_cast<Compound*>( o );
        if ( cpd )
                foreach( DrawObject* co, cpd->childObjects() )
                        removeObjectFromDrawingList( co );
        else
                drawingList_.removeAll( o );
}

/** Sorts the drawingList_. Probably not needed anymore as we have
 *  addObjectToDrawingList().
 */
void Figure::sortObjects()
{
        qSort( drawingList_.begin(), drawingList_.end(), DrawObject::isLessThan );
}

/** Returns the object that intersects with the rectangle of the
 *  center p and the edgelength tolerance.
 */
DrawObject* Figure::findContainingObject( const QPointF& p, qreal tolerance ) const
{
        foreach ( DrawObject* o, objectList_ ) 
                if ( o->pointHits( p, tolerance ) )
                        return o;
        
        return 0;            
}

/** Draws all the DrawObjects of the drawingList_ except those listed
 *  in backups. These are drawn while drawing the Selection.
 */
void Figure::drawElements( QPainter* p, const ObjectList& backups ) const
{
        foreach ( DrawObject* o, drawingList_ )
                if ( !backups.contains( o ) )
                        o->draw( p );
}

/** Outputs the DrawObjects to the OutputBackend ob.
 */
void Figure::outputObjects( OutputBackend* ob ) const
{
        foreach ( DrawObject* o, objectList_ )
                o->outputToBackend( ob );
}

/** Clears objectList_ and drawingList_. All the DrawObjects in the
 *  objectList_ are deleted. Compounds delete their childObjects as
 *  they are their children.
 */
void Figure::clear()
{
        qDeleteAll( objectList_ );
        objectList_.clear();

        drawingList_.clear();
}

/** Returns the smallest rectangle bounding the Figure. Used for
 *  example by exportfilters.
 */
QRectF Figure::boundingRect() const
{
        QRectF r;
        foreach( DrawObject* o, objectList_ ) 
                r |= o->boundingRect();

        return r;
}

/** Each DrawObject of objectList_ is asked for the Resources it uses.
 *  Those resources are then added to the ResourceSet returned if the
 *  Resource::isToBeSaved() and if th Resource is not yet in the
 *  ResourceSet.
 */
const ResourceSet Figure::usedResources() const
{
        ResourceSet resSet;

        foreach ( DrawObject* o, drawingList_ ) {
                ResourceSet rs = o->usedResources();

                for ( ResourceSet::const_iterator it = rs.begin(); it != rs.end(); ++it ) {
                        ResourceKeyList& rkl = resSet[it.key()];

                        foreach ( const ResourceKey& key, it.value() )
                                if ( key.isToBeSaved() && !rkl.contains( key ) )
                                        rkl << key;
                }
        }

        return resSet;
}

