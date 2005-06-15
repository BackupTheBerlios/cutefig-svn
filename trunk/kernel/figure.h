 
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
 *  \brief The Figure contains all DrawObject objects of a figure. It is the 
 *  \e document the user is editing.
 */

#ifndef figure_h
#define figure_h

#include <QList>
#include <QObject>

class DrawObject;
class OutputBackend;
class Controler;

class QPainter;

class QPointF;
class QPoint;
class QRectF;

typedef QList<DrawObject*> ObjectList;
typedef QList<int> DashKeyList;

class Figure : public QObject
{
        Q_OBJECT

public:
        Figure( QObject * parent );
        ~Figure() {};

        void setControler( Controler* c ) { controler_ = c; };
        double scale() const { return scale_; };

        void addDrawObject( DrawObject* drobj, bool fin=false );
        void removeDrawObject( DrawObject* drobj );

        void sortObjects(); //!< Sorts the objects according to their depth.

        void drawElements( QPainter* p, const ObjectList& backups ) const; 
        //!< Draws all objects exept o to the QPainter p.

        DrawObject* findContainingObject( const QPointF& p, qreal tolerance ) const;
        //!< Returns a pointer to the first DrawObject that contains p.

        void outputObjects( OutputBackend* ob ) const;
        //!< Outputs all objects to the OutputBackend ob

        QRectF boundingRect() const;

        void clear();
        //!< Clears the objectList_ and deletes all containing objects.

//         void addDashes( int key ){
//                 dashList_.push_back( key );
//         }; //!< Adds a dash style to the dashList_.

        const DashKeyList dashList() const;

        const ObjectList& objects() { return objectList_; }

private:
        ObjectList objectList_;

        double scale_;

        Controler* controler_;

};
#endif
