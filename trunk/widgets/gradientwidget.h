 
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
        Gradient* gradient_;

        QRect rect1_, rect2_;
        QPointF* movedPoint_;
        QPointF *point1_, *point2_;
        
        void setHandleRects();
        void moveEdgePointTo( QPoint p );
        bool findColorStopUnderMouse( const QPoint& p, bool setIndex = false );
        int colorStopIndex_;
        
        QRect clsToRect( double offset );

        class UIHandler;
//        class DummyUIHandler;
        class LinearUIHandler;
        class RadialUIHandler;

        UIHandler* uiHandler_;
        UIHandler* dummyUIHandler;
        LinearUIHandler* linearUIHandler;
        RadialUIHandler* radialUIHandler;

        AbstractMouseEventHandler* mouseEventHandler_;
        GWMouseEventDispatcher* mouseDispatcher_;
        
        bool initialClick( QMouseEvent* e );
        bool finalClick();
        void drag( QMouseEvent* e );
        void move( QMouseEvent* e );
        
        bool moveInAction_;
        
        void initLinearGradient();
        void initRadialGradient();

        void initUIHandler();

        QSize size_;
        static const int handleSize_ = 5;
};

#endif
