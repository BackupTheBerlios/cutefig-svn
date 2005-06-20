 
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

#ifndef drawobject_h
#define drawobject_h

#include "fig.h"
#include "figure.h"
#include "pen.h"

#include <QRectF>
#include <QPolygonF>
#include <QPainterPath>
#include <QPen>
#include <QBrush>
#include <QRegion>
#include <QObject>

/** \class DrawObject
 *  \brief The baseclass of all objects that can be drawn.
 */

class ObjectDialog;
class OutputBackend;
class CanvasView;
class Compound;

class DrawObject : public QObject
{
        Q_OBJECT
public:
        DrawObject( Figure* parent = 0 );
        DrawObject( DrawObject* );
        ~DrawObject() {}

        friend void Figure::addDrawObject( DrawObject* , bool );
        friend class Compound;

        virtual DrawObject* copy() = 0;
        //!< supposed to make a deep copy and return a pointer to it.

        virtual const QString objectname() =0;
        //!< supposed to return the name of the object in english.

        virtual bool pointHits( const QPointF& p, qreal tolerance ) const;
        virtual bool pointHitsOutline( const QPointF&, qreal tolerance ) const = 0;

        QRectF boundingRect() const { return bRect_; }
        //!< Returns the smallest QRectF that bounds the object.

        const Pen& pen() const { return pen_; }
//         QColor penColor() const { return pen_.color(); }
//         qreal penWidth() const { return pen_.width(); }
//         Qt::PenCapStyle penCapStyle() const { return pen_.capStyle(); }
//         Qt::PenJoinStyle penJoinStyle() const { return pen_.joinStyle(); }

        QColor fillColor() const { return fillColor_; } 

        int depth() const { return depth_; }

        void setComment( const QString& str ) { commentString_ = str; }
        QString comment() const { return commentString_; }

        void setCurrentPoint( QPointF* p ) { currentPoint_ = p; }
        //!< Tells the object which point is to be edited.
        
        void setPoints( QPolygonF pa ) { points_ = pa; }
        QPolygonF& points() { return points_; }
        virtual QPointF center() const { return points_.boundingRect().center(); }
        //!< returns the central point of the object.

        virtual void draw( QPainter* p );
        //!< supposed to draw the object to the QPainter p.

        virtual void drawTentative( QPainter* p );

        bool pointSet( const QPointF & pos, Fig::PointFlag f = Fig::Normal ); 
        //!< sets the point just being edited and returns the next point.

        void cursorMove( const QPointF & pos );
        //!< moves the point just being edited tentatively.

        virtual void move( const QPointF& d );
        //!< moves the object by dx and dy

        virtual void mapMatrix( const QMatrix& m );
        //!< scales the object by dx and dy

        QRect selectPointRect( QPoint p ) const 
        { return QRect( p.x()-4, p.y()-4, 8,8 ); }

        virtual void outputToBackend( OutputBackend* ob ) = 0;
        //!< supposed to pass all the data to the OubputBackend ob.
        /*!< This is to be done by calling ob->output<DerivedClass>(). */

        static bool isLessThan( DrawObject* o1, DrawObject* o2 ) { 
                return o1->depth() < o2->depth();
        }


signals:
        void objectChanged();
        //!< supposed to be emited when the object changed.

public slots:
//         void setPenWidth( qreal pw ) { pen_.setWidth( pw ); }
//         void setPenColor( const QColor& pc );

        void setPen( const Pen& p ) { pen_ = p; }

        void setFillColor( const QColor& c );
        
        void setDepth( int d ) { depth_ = d; }

protected:
        virtual QPointF* nextPoint() = 0;
        //!< supposed to return the next point to be edited if there is one.

        virtual void passPointFlag( Fig::PointFlag f ) =0;
        //!< supposed to handle the PointFlag appropriately

        virtual void getReadyForDraw() = 0;
        //!< supposed to do all preparations for the drawing.
        /*!< Especially bRect_ and dRect_ are to be set. */

        virtual void doSpecificPreparation() {};

        void addSelPointsToDrawRect();
        //!< adds the small squares indicationg the points_ to the dRect_.


        Figure* figure_;

        Pen pen_;
        QPen qpen_;
        QBrush brush_;
        QColor fillColor_;

        int depth_;

        QString commentString_;

        QPainterPath painterPath_;
        QPolygonF points_;
        QPointF* currentPoint_;
        QPointF centerPoint_;

        QRectF bRect_;
        QRect drawRect_;
        static unsigned clickTolerance_;

};


#endif
