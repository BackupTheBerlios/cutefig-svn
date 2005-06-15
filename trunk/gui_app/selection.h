 
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

/** \class Selection
 *
 *  \brief A class to remember the objetcs the user has selected.
 *
 *  The Selection basically has two ObjectLists: backups_ are the
 *  objects that are currently in the Figure. objects_ contains the
 *  objects that can be edited.
 */

#ifndef drobjectgroup_h
#define drobjectgroup_h

#include <QList>
#include <QObject>

class DrawObject;
class QRegion;
class QRectF;
class QPainter;
class QPointF;

typedef QList<DrawObject*> ObjectList;

class Selection : public QObject
{
        Q_OBJECT
public:
        Selection()
                : QObject(),
                  objects_(),
                  backups_() {}

        ~Selection() {}

//        void selectList( const ObjectList& l ); // probably not needed
        bool select( DrawObject* o );
        void setObjectToBeCreated( DrawObject* o, bool destructive = true );
        void setListToBeInserted( const ObjectList& l, bool destructive = true );

        void clear( bool destructive = true );
       
        bool isEmpty() const { return objects_.isEmpty(); }

        const ObjectList& objects() const { return objects_; }
        const ObjectList& backups() const { return backups_; }

        void restoreBackups();
        void updateBackups();
        void renewBackups();

        const QString objectname() const;

//        const QRegion region() const; // probably not needed
        const QRectF boundingRect() const;

signals:
        void selectionChanged();

private:

        ObjectList objects_, backups_; 
};

#endif
