 
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
#include "stroke.h"
#include "typedefs.h"

#include <QRectF>
#include <QPolygonF>
#include <QPainterPath>
#include <QPen>
#include <QBrush>
#include <QRegion>
#include <QObject>

#include <QDebug>

/** \class DrawObject
 *  \brief The baseclass of all objects that can be drawn.
 */

class ObjectDialog;
class OutputBackend;
class CanvasView;
class Compound;
class AbstractResourceUser;

class DrawObject : public QObject
{
        Q_OBJECT
public:
        DrawObject( Figure* parent = 0 );
        DrawObject( const DrawObject* );
        ~DrawObject() {}

        friend void Figure::takeDrawObjects( const ObjectList& );
        friend class Compound;

        //! supposed to make a deep copy and return a pointer to it.
        virtual DrawObject* copy() const = 0;
        
        //! supposed to return the name of the object in english.
        virtual const QString objectname() const =0;
        virtual const QString objectKeyWord() const =0;

        DrawObject* ancestor();

        virtual uint minPoints() const { return 2; }
        
        virtual bool pointHits( const QPointF& p, qreal tolerance ) const;
        virtual bool pointHitsOutline( const QPointF& p, qreal tolerance ) const =0;
        
        QRectF boundingRect() const { return bRect_; }
        //!< Returns the smallest QRectF that bounds the object.

        QRectF controlPointRect() const { return cRect_; }
        
        const Pen& pen() const { return pen_; }
        Pen* p_pen() { return &pen_; }
        
        const Stroke& stroke() const { return stroke_; }
        Stroke* p_stroke() { return &stroke_; }
        
        const Stroke& fill() const { return fill_; }
        Stroke* p_fill() { return &fill_; }
        
        int depth() const { return depth_; }

        void setComment( const QString& str ) { commentString_ = str; }
        QString comment() const { return commentString_; }

        void setCurrentPoint( QPointF* p ) { currentPoint_ = p; }
        //!< Tells the object which point is to be edited.
        
        void setPoints( QPolygonF pa ) { points_ = pa; }
        QPolygonF& points() { return points_; }
        const QPolygonF& points() const { return points_; }
        virtual QPointF center() const { return points_.boundingRect().center(); }
        //!< returns the central point of the object.

        virtual void draw( QPainter* p ) const;
        //!< supposed to draw the object to the QPainter p.

        virtual void drawTentative( QPainter* p ) const;

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

        virtual const ResourceSet usedResources();
        virtual void releaseResources();
        virtual void reclaimResources();
        
        virtual void outputToBackend( OutputBackend* ob ) = 0;
        //!< supposed to pass all the data to the OubputBackend ob.
        /*!< This is to be done by calling ob->output<DerivedClass>(). */

        static bool isLessThan( const DrawObject* o1, const DrawObject* o2 ) { 
                return o1->depth() > o2->depth();
        }

public slots:
        void setPen( const Pen& p );
        void setStroke( const Stroke& s ) { stroke_ = s; }
        void setFill( const Stroke& s ) { fill_ = s; }
//        void setFillColor( const QColor& c );
        
        void setDepth( int d ) { depth_ = d; }

protected:
        virtual QPointF* nextPoint() = 0;
        //!< supposed to return the next point to be edited if there is one.

        virtual void passPointFlag( Fig::PointFlag f ) =0;
        //!< supposed to handle the PointFlag appropriately

        virtual void setupPainterPath() = 0;
        //!< supposed to do all preparations for the drawing.
        /*!< Especially bRect_ and cRect_ are to be set. */

        virtual void doSpecificPreparation() {}

        virtual void getReadyForDraw();

        virtual void setupRects();

        virtual void addSpecificResources( ResourceSet& ) {}

        virtual QList<AbstractResourceUser*> resourceUsers();


        Figure* figure_;

        Pen pen_;
        Stroke stroke_, fill_;

        int depth_;

        QString commentString_;

        QPainterPath painterPath_;
        QPolygonF points_;
        QPointF* currentPoint_;
        QPointF centerPoint_;

        QRectF bRect_, cRect_;
        static unsigned clickTolerance_;

private:
        void setCompoundParent( Compound* p );
        Compound* compoundParent_;
};

#define CUTE_DECLARE_DRAWOBJECT( Classname, okeyword, oname ) \
class Classname; \
namespace DObjects \
{ \
         template<> inline const QString objectname<Classname>() \
         { return DrawObject::tr(oname); } \
         template<> inline const QString objectKeyWord<Classname>() { return okeyword; } \
}

#define DRAW_OBJECT( Classname ) \
public:\
         const QString objectname() const { return DObjects::objectname<Classname>(); } \
         const QString objectKeyWord() const { return DObjects::objectKeyWord<Classname>(); } \
         DrawObject* copy() const { return new Classname( this ); } \
private:

#endif
