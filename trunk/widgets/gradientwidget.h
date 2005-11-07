 
/*****************************************************************************
**
**  $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2005 Johannes Mueller, johmue@users.sourceforge.net
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

class GradientWidget : public QWidget
{
        Q_OBJECT
public:
        GradientWidget( Gradient* gr, QWidget* parent =0 );
        ~GradientWidget();

        friend class GWMouseEventDispatcher;

        void setGradient( Gradient* gr );
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
        
        void (GradientWidget::*pointSignalX)( double );
        void (GradientWidget::*pointSignalY)( double );

        QRect rect1_, rect2_;
        QPointF* movedPoint_;
        QPointF *point1_, *point2_;
        
        void setHandleRects();
        void moveEdgePointTo( QPoint p );
        bool findColorStopUnderMouse( const QPoint& p, bool setIndex = false );
        int colorStopIndex_;
        
        QRect clsToRect( double offset );

        class UIHandler;
        class DummyUIHandler;
        class LinearUIHandler;
        class RadialUIHandler;

        UIHandler* uiHandler_;
        DummyUIHandler* dummyUIHandler;
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
