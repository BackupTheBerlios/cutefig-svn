
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
#include "arrow.h"
#include "typedefs.h"

#include <QRectF>
#include <QPolygonF>
#include <QPainterPath>
#include <QPen>
#include <QBrush>
#include <QRegion>
#include <QObject>

#include <QDebug>


class Compound;
class ObjectDialog;
class OutputBackend;
class CanvasView;
class AbstractResourceUser;

//! The baseclass of all objects that can be drawn.
/*!
 *  \section intro Introduction
 *  
 *  It contains all the basic data of a DrawObject like pen_,
 *  commentString_... 
 *
 *  Many functions are virtual or even pure virtual as it is the
 *  subclasses job to implement them.
 *
 *  
 *  \section subclassing Issues when subclassing
 *  
 *  Each kind of DrawObject, i.e. each concrete subclass has a keyword
 *  and a name.
 *
 *  The keyword is used to refer to the subclass internally and in the
 *  CuteFig data file. Usually instances all classes dealing with
 *  DrawObjects subclasses are created by factories, that are looked
 *  up by the keyword. As the keyword is used in the data files it
 *  should be short and should only consist of small letters.
 *
 *  The name is displayed to the user. So it should be the english
 *  name for an Object represented by the subclass. It also should
 *  contain a '&' for keyboard acceleration an should start with a
 *  capital letter.
 *
 *  The usual way to define keyword and name is the use of the macro
 *  #DRAW_OBJECT in the class definition of DrawObject's subclass. This
 *  define also declares and defines the function copy() that is used
 *  to clone a DrawObject.
 *
 *  
 *  \section attributes Important attributes
 *  
 *  Every DrawObject has the following important attributes that are
 *  accessible by public setters and getters and are also written to
 *  the data file.
 *      - #pen_ 
 *      - #stroke_ 
 *      - #fill_ 
 *      - #depth_
 *      - #commentString_
 *
 *  Other attributes that are publicly accessible readonly:
 *      - #bRect_
 *      - #cRect_
 *
 *
 *  \section calculating Calculating issues
 *  
 *  Every time something in the DrawObject changes it has to
 *  recalculate itself. This is done by calling getReadyForDraw()
 *  which then calls setupPainterPath() and setupRects(). All of them
 *  can be reimplemented. Reimplementing getReadyForDraw() is not
 *  recommended though.
 *
 *  Things in the DrawObject that need not be recalculated after
 *  tentative changes can be done in the method doSpecificPreparation().
 *
 *
 *  \section Drawing
 *
 *  There are two methods that are responsible for drawing:
 *      - draw()
 *      - drawTentative()
 *
 *  The latter draws the object without fill and with a default pen,
 *  so that Strokes don't have to be rendered. Usually the two classes
 *  don't have to be reimplemented as the #painterPath_ that is
 *  calculated in setupPaiterPath_ is used. Some things are not
 *  drawable by QPainterPath though, e.g. rich text.
 *
 *
 *  \section editing Editing the object
 *
 *  \subsection creating Creating an a DrawOject
 *
 *  The following methods are the public interface to create a
 *  DrawObject by user interaction.
 *     - pointSet()
 *     - cursorMove()
 *
 *  \subsubsection public Public interface
 *  Creating a DrawObject by user inteaction works the following way:
 *  A an object of the specific DrawObject subclass is created by the
 *  CreateAction as soon as the activates the CreateAction. Then the
 *  CreateAction takes the users's mouse clicks and passes the points
 *  on to the DrawObject by calling pointSet(). If the DrawObject
 *  considers itself as finished pointSet() will return false
 *  otherwise true. So the CreateAction will be active until
 *  pointSet() returns false and will then add the DrawObject to the
 *  Figure by AddCommand. During the CreateAction is active it passes
 *  mouse moves to the DrawObject by calling cursorMove(). 
 *
 *  \subsubsection private Behind the scenes  
 *  The following functions must be reimplemented by subclasses to
 *  handle userinteraction.
 *      - nextPoint()
 *      - passPointFlag()
 *  
 *  
 *  \section Resources
 */
class DrawObject : public QObject
{
	Q_OBJECT
public:
        explicit DrawObject( Figure* parent = 0 );
        DrawObject( const DrawObject* );
        ~DrawObject() {}

	friend class Compound;

        //! Usually reimplemented by #DRAW_OBJECT
        virtual DrawObject* copy() const = 0;
        
        //! Usually reimplemented by #DRAW_OBJECT
        virtual const QString& objectname() const =0;
        //! Usually reimplemented by #DRAW_OBJECT
        virtual const QString& objectKeyWord() const =0;

        QString objectname_stripped() const;

        //! returns the highest Compound the object belongs to or the object itself.
        const DrawObject* ancestor() const;

        //! retrurns a reference to the figure
        const Figure& figure() const { return *figure_; }

        //! returns the minimum number of points the objetct need to be defined.
        virtual int minPoints() const { return 2; }

        //! returns true if the DrawObject type can have arrows.
        virtual bool canHaveArrows() const { return true; }

        //! returns true if point \em p is a point to grasp the object.
        virtual bool pointHits( const QPointF& p, qreal tolerance ) const;

        //! supposed to return true in case point \p hits the outline of the object.
        virtual bool pointHitsOutline( const QPointF& p, qreal tolerance ) const;
        
        //! Returns the smallest QRectF that bounds the object.
        QRectF boundingRect() const { return bRect_; }

        //! returns the smallest QRectF that contains all #points_
        QRectF controlPointRect() const { return cRect_; }

        
        const Pen& pen() const { return pen_; }
        //! sets the #pen_
        void setPen( const Pen& p );
        
        const Stroke& stroke() const { return stroke_; }
        //! sets the outline #stroke_
        void setStroke( const Stroke& s ) { stroke_ = s; }

        
        const Stroke& fill() const { return fill_; }
        //! sets the fill_ #stroke
        void setFill( const Stroke& s ) { fill_ = s; }

        
        int depth() const { return depth_; }
        //! sets the #depth_
        void setDepth( int d ) { depth_ = d; }


        void setComment( const QString& str ) { commentString_ = str; }
        QString comment() const { return commentString_; }

        virtual void setCurrentPointIndex( int i );
        //!< Tells the object which point is to be edited.
        
        void setPoints( const QPolygonF& pa ) { points_ = pa; }

	//! Returns the control points of the object.
        const QPolygonF& points() const { return points_; }

	void appendPoint( const QPointF& p );

        //! returns the central point of the object.
        virtual QPointF center() const { return points_.boundingRect().center(); }

        //! returns
        const Arrow& startArrow() const { return startArrow_; }
        const Arrow& endArrow() const { return endArrow_; }

        const ResourceKey& startArrowKey() const { return startArrow_.key(); }
        const ResourceKey& endArrowKey() const { return endArrow_.key(); }
        
        void setStartArrow( const Arrow& a ) { startArrow_ = a; }
        void setEndArrow( const Arrow& a ) { endArrow_ = a; }

        void setStartArrow( const ResourceKey& k ) { startArrow_.setArrowPainter( k ); }
        void setEndArrow( const ResourceKey& k ) { endArrow_.setArrowPainter( k ); }
        

        //! supposed to draw the object to the QPainter p.
        virtual void draw( QPainter* p ) const;

        //! supposed to draw the object tentatively, i.e. while the user is handling it.
        virtual void drawTentative( QPainter* p ) const;

        //! supposed to draw object's meta data such as helplines
        virtual void drawMetaData( QPainter* ) const {}
        
        //! sets the point just being edited and returns true if another point is needed.
        bool pointSet( const QPointF & pos, Fig::PointFlags f = Fig::Normal ); 

        //! moves the point just being edited tentatively.
        virtual void cursorMove( const QPointF & pos );

        //! moves the object by dx and dy
        virtual void move( const QPointF& d );

        //! scales the object by dx and dy
        virtual void mapMatrix( const QMatrix& m );

        //! returns a list of all resources, the object is using.
        virtual const ResourceSet usedResources() const;

        //! releases all the resources the object uses
        /*! usually called on removal of an object */
        virtual void releaseResources() const;

        //! reclaims all the resources the object uses.
        /*! usually called on readding the object, e.g. by undo/redo
         *  or by paste. */
        virtual void reclaimResources() const;
        
        //! supposed to pass all the data to the OubputBackend ob.
        /*! This is to be done by calling ob->output<DerivedClass>(). */
        virtual void outputToBackend( OutputBackend* ob ) const = 0;


	void update();
	void updateEverything();
	
	
        //! used to sort the DrawObjects according to their #depth_
        static bool isLessThan( const DrawObject* o1, const DrawObject* o2 )
        { 
                return o1->depth() > o2->depth();
        }

//public slots:
  
protected:
        //! not meant to be called manually but by #DRAW_OBJECT
        template<typename OT> DrawObject* doCopy( const OT* orig ) const { return new OT( orig ); }
        
        //! supposed to return the index of next point to be edited ot -1
        virtual int nextPointIndex() = 0;

        //! supposed to handle the PointFlag appropriately
        virtual void passPointFlag( Fig::PointFlags f ) =0;

        //! supposed to do all preparations for the drawing.
        virtual void setupPainterPath() = 0;

        //! supposed to calculate things to be recalculated after non tentative changes.
        virtual void doSpecificPreparation() {}

        //! recalculates things after any change. Usually not to be reimplemented.
        virtual void getReadyForDraw();

        //! supposed to set bRect_ and cRect_ 
        virtual void setupRects();

        //! supposed to add subclass specific ResourceUsers to the QList passed.
        virtual void addSpecificResourceUsers( QList<AbstractResourceUser*>& ) const {}

        //! returns all resourcUsers the object has.
        virtual QList<AbstractResourceUser*> resourceUsers() const;

        int currentPointIndex() const { return currentPointIndex_; }


        void drawArrows( QPainter* p ) const;

        virtual QPointF startAngle( const QPolygonF& pol ) const;
        virtual QPointF endAngle( const QPolygonF& pol ) const;

	bool event( QEvent* e );
        
        //! the Figure containing the DrawObject
        Figure* figure_;

        //! the Pen of the object
        Pen pen_;

        //! the Stroke used for the outline of the DrawObject
        Stroke stroke_;

        //! the Stroke used for the filling of the DrawObject
        Stroke fill_;

        //! the depth of the object.
        /*! "deeper" objects are (partially) covered by "not as deep"
         *  objects Which object is covered in case two overlapping
         *  object have the same depth is undefined.
         */
        int depth_;

        //! the comment of an object (no relevance to drawing)
        QString commentString_;

        QPainterPath painterPath_;
        QPolygonF points_;
        QPointF centerPoint_;

        QRectF bRect_, cRect_;


private:
        int currentPointIndex_;
        void setCompoundParent( Compound* p );
        Compound* compoundParent_;

        Arrow startArrow_;
        Arrow endArrow_;

	bool updateEverything_;
};

//! Small helper class to store the DrawObject's keyword and name
/*! This class is not meant to be instaciated manually but by the
 *  compiler macro #DRAW_OBJECT. It is inspired by the Q_OBJECT macro
 *  of Qt.
 */
class MetaObject 
{
public:
        MetaObject( const QString& name, const QString& keyword )
                : name_( name ),
                  keyWord_( keyword )
        {}

        const QString& name() const { return name_; }
        const QString& keyWord() const { return keyWord_; }

private:
        MetaObject( const MetaObject& );
        MetaObject& operator=( const MetaObject& );
        QString name_, keyWord_;
};

//! Defines things a concrete subclass of DrawObject needs.
/*! Its parameters are the keyword and the name of the DrawObject's
 *  subclass using a static instance of MetaObject. This instance is
 *  accessible by the static method <Subclass>::cuteMetaObject().
 *
 *  Note that DrawObject itself does not have a method cuteMetaObject().
 *
 *  In order to get a concrete DrawObject's name and keyword there are
 *  two ways:
 *
 *      - If there is an existing object of a subclass of DrawObject
 *      use the vitual methods DrawObject::objectname() and
 *      DrawObject::keyword().
 *
 *      - If there is no object but only the type, e.g. in templates
 *      use the static method <Subclass>::cuteMetaObject() to get a
 *      refenrence to the MetaObject. Then use MetaObject::name() and
 *      MetaObject::keyword().
 *
 *  This macro is inspired by Qt's Q_OBJECT macro.
 */
#define DRAW_OBJECT( okeyword, oname ) \
public: \
        static const MetaObject& cuteMetaObject() \
        { static MetaObject mo( oname, okeyword ); return mo; }\
        const QString& objectname() const { return cuteMetaObject().name(); } \
        const QString& objectKeyWord() const { return cuteMetaObject().keyWord(); } \
        DrawObject* copy() const { return doCopy( this ); } \
private:

        
#endif
