 
/*****************************************************************************
**
**  $Id$
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

/** \page mouse_interaction Handling of mouse interaction
 *
 *  As the CuteFig is inspired by the XFig drawing programmme, it
 *  handles the mouse interaction similar as XFig does, i.e. it more
 *  deals with mouse clicks rather than with mouse drags.
 *
 *  In order to make it easy for users coming from other drawing
 *  programmes like for example CorelDraw to adapt to the CuteFig we
 *  need a mechanism to switch between the two ways of mouse
 *  using. Therfore we use the class hierachy
 *  AbstractMouseEventHandler and AbstractMouseEventDispatcher.
 *
 *  \section mouse_interaction_mechanism Mechanism
 *
 *  Any widget that needs mouse interaction should
 *  come up with a subclass of AbstractMouseEventDispacher and
 *  subclass the pure virtual methods. In its constructor it sould
 *  pass an instance of this MouseEventDispatcher to the
 *  MouseEventHandlerFactory which is still to be written :).
 *
 *  The MouseEventHandlerFactory gives it back an sublcass of
 *  AbstractMouseEventHandler. To this MouseEventHandler any
 *  QMouseEvent that is to be handled by the widget should be passed.
 *
 *  The MouseEventHandler then calls the methods of the widget's
 *  MouseEventDispatcher. Those methods are supposed to inform the
 *  widget about the user's mouse interactions.
 *
 *  By now there are the following types of mouse interactions
 *  foreseen.
 *    - initialClick()
 *    - finalClick()
 *    - drag()
 *    - move()
 */

#ifndef mouseeventhandler_h
#define mouseeventhandler_h

class QMouseEvent;


//! Abstract baseclass of MouseEventDispatchers.
/*!
 *  It is to be subclassed for all widgets that need mouse
 *  interaction. It contains only pure virtual methods that are to be
 *  reimplemented in a way that they inform the widget that the
 *  subclass is assigned to about the user's mouse interaction.
 */
class AbstractMouseEventDispatcher
{
public:
        //! The QMouseEvent initiating the "drag"
        /*! Supposed to return true in case a dragging really occurs
         *  after the mouse event.
         */
        virtual bool initialClick( QMouseEvent* e ) = 0;

        //! The QMouseEvent finishing the "drag"
        /*! Supposed to return true in case a dragging really occurs
         *  after the mouse event.
         */
        virtual bool finalClick( QMouseEvent* e ) = 0;

        //! The QMouseEvent of a "drag"
        virtual void drag( QMouseEvent* e ) = 0;

        //! The QMouseEvent of a simple mouse move without "dragging" anything
        virtual void move( QMouseEvent* e ) = 0;
};


//! Abstract baseclass of MouseEventHandlers
/*!
 *  This class is to be subclassed when implementing a new behavior of
 *  mouse interaction.
 */
class AbstractMouseEventHandler
{
public:
        //! Needs an instance of AbstractMouseEventDispatcher.
        /*! This event dispatcher is called when mouse interaction takes place.
         */
        AbstractMouseEventHandler( AbstractMouseEventDispatcher* dp )
                : dispatcher_( dp ),
                  dragging_( false )
        {}

        //! To be called by MyWidget::mousePressEvent()
        virtual void mousePressEvent( QMouseEvent* e ) = 0;

        //! To be called by MyWidget::mouseMoveEvent()
        virtual void mouseMoveEvent( QMouseEvent* e );

        //! To be called by MyWidget::mouseReleaseEvent()
        virtual void mouseReleaseEvent( QMouseEvent* e ) = 0;

protected:
        //! The event dispatcher that will be informed about mouse events.
        AbstractMouseEventDispatcher* dispatcher_;

        //! True if the mouse next move event is dragging something.
        bool dragging_;
};


//! Mimics mouse behaviour known from XFig.
/*!
 * A mouse "drag" is considered to be a mouse move in between two clicks.
 */
class ClickMouseEventHandler : public AbstractMouseEventHandler
{
public:
        ClickMouseEventHandler( AbstractMouseEventDispatcher* dp )
                : AbstractMouseEventHandler( dp )
        {}
        
        virtual void mousePressEvent( QMouseEvent* ) {}
        virtual void mouseReleaseEvent( QMouseEvent* e );
};

class DragMouseEventHandler : public AbstractMouseEventHandler
{
public:
        DragMouseEventHandler( AbstractMouseEventDispatcher* dp )
                : AbstractMouseEventHandler( dp )
        {}
        
        virtual void mousePressEvent( QMouseEvent* e );
        virtual void mouseReleaseEvent( QMouseEvent* e );
};


#endif
