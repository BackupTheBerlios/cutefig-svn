 
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

#include "selection.h"
#include "drawobject.h"

#include <QRectF>

#include <QDebug>

/**
 */
// void Selection::selectList( const ObjectList& l )
// {
//         backups_ = l;
//         objects_ = ObjectList();
//         foreach ( DrawObject* o, l )
//                 objects_.append( o->copy() );
// }

/** Unselects everything on receiving a null pointer. Otherwise the
 *  DrawObject o is added to the Selection if it is not yet in it. If
 *  it is it is unselected.
 */
bool Selection::select( DrawObject* o )
{
        if ( !o ) {
                if ( isEmpty() )
                        return false;
                clear();
        }
        
        else if ( backups_.contains( o ) ) {
                objects_.removeAt( backups_.indexOf( o ) );
                backups_.removeAll( o );
        } else {
                backups_.append( o );
                objects_.append( o->copy() );
        }

        emit selectionChanged();
        return true;
}

/** To be called if a new object is createt, e.g by a CreateAction. In
 * this case the backups_ are empty because it doesn't make sense to
 * copy a DrawObject, that is not yet in the Figure.
 */
void Selection::setObjectToBeCreated( DrawObject* o, bool destructive )
{
        if ( destructive ) 
                clear();
        else 
                objects_.clear();
        

        objects_.append( o );

        emit selectionChanged();
}

/** Similar to setObjectToBeCreated() but it takes a ObjectList. It's
 * useful if several DrawObjects are to be inserted at once, e.g. by a
 * PasteAction.
 */
void Selection::setListToBeInserted( const ObjectList& l, bool destructive )
{
        if ( destructive ) 
                clear();
        else
                objects_.clear();

        objects_ = l;

        emit selectionChanged();
}

/** Clears the selection. If destructive is true (default), the
 *  selected objects (not the backups) are really deleted.
 */
void Selection::clear( bool destructive )
{
        if ( destructive )
                qDeleteAll( objects_ );
        backups_.clear();
        objects_.clear();
}

/** Deletes the objects and copies it again from the backups
 */
void Selection::restoreBackups()
{
        qDeleteAll( objects_ );
        objects_.clear();
        foreach( DrawObject* o, backups_ )
                objects_.append( o->copy() );
}

/** Copies the backups_ to objects_. Useful after an InteractiveAction
 * is completed.
 */
void Selection::updateBackups()
{
        backups_ = objects_;
        renewBackups();
}

/** regenerates the objects_ out of the backups_
 */
void Selection::renewBackups()
{
        objects_.clear();
        foreach( DrawObject* o, backups_ )
                objects_.append( o->copy() );
}

const QString Selection::objectname() const
{
        if ( objects_.count() == 1 )
                return objects_[0]->objectname();
        else
                return "selection";
}

// const QRegion Selection::region() const
// {
//         QRegion r;
//         foreach ( DrawObject* o, objects_ )
//                 r |= o->region();

//         return r;
// }

/** Returns the boundingRect of all objects.
 */
const QRectF Selection::boundingRect() const
{
        QRectF r;
        foreach ( DrawObject* o, objects_ )
                r |= o->boundingRect();

        return r;
}

