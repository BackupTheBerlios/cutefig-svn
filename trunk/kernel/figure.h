 
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
 *  \brief The Figure contains all DrawObject objects of a figure. It is the 
 *  \e document the user is editing.
 */

#ifndef figure_h
#define figure_h

#include <QList>
#include <QObject>

#include "typedefs.h"

class DrawObject;
class OutputBackend;
class Controler;

class QPainter;

class QPointF;
class QPoint;
class QRectF;


class Figure : public QObject
{
        Q_OBJECT

public:
        Figure( QObject * parent = 0 );
        ~Figure() {};

        void setControler( Controler* c ) { controler_ = c; };
        double scale() const { return scale_; };

        void takeDrawObjects( const ObjectList& l );
        void addDrawObject( DrawObject* drobj );
        void removeDrawObject( DrawObject* drobj );

        void sortObjects(); 

        void drawElements( QPainter* p, const ObjectList& backups = ObjectList() ) const; 

        DrawObject* findContainingObject( const QPointF& p, qreal tolerance ) const;

        void outputObjects( OutputBackend* ob ) const;
        //!< Outputs all objects to the OutputBackend ob

        QRectF boundingRect() const;
        bool isEmpty() const { return objectList_.isEmpty(); }
                

        void clear();
        //!< Clears the objectList_ and deletes all containing objects.

//         void addDashes( int key ){
//                 dashList_.push_back( key );
//         }; //!< Adds a dash style to the dashList_.

        const ResourceSet usedResources() const;
        
//         const ResourceKeyList dashList() const;
//         const ResourceKeyList strokeList() const;

        const ObjectList& objects() { return objectList_; }

private:
        void addObjectToDrawingList( DrawObject* o );
        void removeObjectFromDrawingList( DrawObject* o );
        void sortIntoDrawingList( DrawObject* o );
        
        ObjectList objectList_, drawingList_;

        double scale_;

        Controler* controler_;

};
#endif
