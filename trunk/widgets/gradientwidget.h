 
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

#ifndef gradientwidget_h
#define gradientwidget_h

#include <QWidget>
#include <QPixmap>
#include <QGradient>

class Gradient;
class QPainter;
class AbstractMouseEventHandler;
class GWMouseEventDispatcher;

//! A widget to edit gradients.
/*!
 *  Takes a pointer to a Gradient and lets the user edit it. Note that
 *  no backup of the gradient is made. It uses the \ref
 *  mouse_interaction "mouse interaction" to deal with mouse events.
 *
 *  Inside GradientWidget there is a class hierachy based on
 *  UIHandler. Those are to distinguish between Gradient::Linear and
 *  Gradient::Radial.
 */
class GradientWidget : public QWidget
{
        Q_OBJECT
public:
        //! a pointer to the gradient to be edited.
        GradientWidget( Gradient* gr, QWidget* parent =0 );
        ~GradientWidget();

        //! the subclass of AbstractMouseEventDispatcher that deals with mouse events
        friend class GWMouseEventDispatcher;

        //! changes the gradient to be edited.
        void setGradient( Gradient* gr );

        //! reimplemented from QWidget
        virtual void update();

protected:
        virtual void paintEvent( QPaintEvent* e );
        virtual void mouseReleaseEvent( QMouseEvent* e );
//        virtual void mousePressEvent( QMouseEvent* e );
        virtual void mouseMoveEvent( QMouseEvent* e );

        virtual void resizeEvent( QResizeEvent* );
        virtual void contextMenuEvent( QContextMenuEvent* e );

        virtual void changeEvent( QEvent* e );
        
private:
        //! The Gradient being edited
        Gradient* gradient_;

        //! The two rectangles representing the two points defining the gradient
        QRect rect1_, rect2_;

        //! a pointer to the point being moved
        QPointF* movedPoint_;

        //! pointers two the points defining the gradient
        QPointF *point1_, *point2_;

        
        //! sets up rect1_ and rect2_
        void setHandleRects();


        //! moves *movedPoint_ to p assuring that it's not "outside" of the widget
        void moveEdgePointTo( QPoint p );

        //! returns true if there is a color stop under the mouse cursor.
        bool findColorStopUnderMouse( const QPoint& p, bool setIndex = false );

        //! the index of the color stop just being handled
        int colorStopIndex_;

        //! returns a rectangle that can be used to represent a color stop at offset.
        QRect clsToRect( double offset );

        class UIHandler;
        class LinearUIHandler;
        class RadialUIHandler;

        UIHandler* uiHandler_;
        UIHandler* dummyUIHandler;

        AbstractMouseEventHandler* mouseEventHandler_;
        GWMouseEventDispatcher* mouseDispatcher_;

        //! handles the initial click of a drag event.
        bool initialClick( QMouseEvent* e );

        //! handles the final click of a drag event.
        bool finalClick();

        //! handles the mouse move of a drag event.
        void drag( QMouseEvent* e );

        //! handles the mous move without dragging anything
        void move( QMouseEvent* e );

        //! makes uiHandler_ point to a LinearUIHandler
        void initLinearGradient();
        
        //! makes uiHandler_ point to a RadialUIHandler
        void initRadialGradient();

        //! decides which kind of UIHandler to use
        void initUIHandler();

        //! the widget size() - 2 handleSize_, so that the Gradient
        //  does not use up all the widget area.
        QSize size_;

        //! TODO the size of the handles... to be outsourced somewhere else.
        static const int handleSize_ = 5;
};

#endif
