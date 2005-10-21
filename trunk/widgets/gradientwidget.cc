 
/*****************************************************************************
**
**  $Id: gradientwidget.cc,v 1.3 2004/11/01 22:38:41 mueller Exp $
**
**  This file is part of CuteFig
**
**  Copyright (C) 2004 Johannes Mueller, johmue@users.sourceforge.net
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

//#include <QtGui>

#include "gradientwidget.h"
#include "gradient.h"
#include "geometry.h"
#include "mouseeventhandler.h"

#include <QPainter>
#include <QPaintEvent>
#include <QColorDialog>

#include <cmath>

#include <QDebug>

class GradientWidget::UIHandler
{
public:
        UIHandler( GradientWidget& w ) : s( w ) {}
        virtual bool click( const QPoint& ) { return false; }
        virtual void move( const QPoint& ) {}
        virtual void paint( QPainter* p ) = 0; 
        virtual bool colorStopHit( const QGradientStop& gs, const QPoint& p ) =0;
        virtual double calcOffset( const QPoint& p ) = 0;
        virtual bool isReal() const = 0;
//        virtual void reset() {}
        
protected:
        GradientWidget& s;
};

class GradientWidget::DummyUIHandler : public GradientWidget::UIHandler 
{
public:
        DummyUIHandler( GradientWidget& w ) : UIHandler( w ) {}
        virtual void paint( QPainter* ) {}
        virtual bool colorStopHit( const QGradientStop&, const QPoint& ) { return false; }
        virtual double calcOffset( const QPoint& ) { return 0; }
        virtual bool isReal() const { return false; }
};

class GradientWidget::LinearUIHandler : public GradientWidget::UIHandler
{
public:
        LinearUIHandler( GradientWidget& w ) : UIHandler( w ) {}
        virtual void paint( QPainter* p );
        virtual bool colorStopHit( const QGradientStop& gs, const QPoint& p );
        virtual double calcOffset( const QPoint& p );
        virtual bool isReal() const { return true; }
};

class GradientWidget::RadialUIHandler : public GradientWidget::UIHandler
{
public:
        RadialUIHandler( GradientWidget& w )
                : UIHandler( w ) {}
        virtual bool click( const QPoint& );
        virtual void move( const QPoint& );
        virtual void paint( QPainter* p );
        virtual bool colorStopHit( const QGradientStop& gs, const QPoint& p );
        virtual double calcOffset( const QPoint& p );
        virtual bool isReal() const { return true; }
//        virtual void reset() {}

        bool radiusGrasped( const QPoint& p, const QPoint& c, qreal radius );

private:
        QPoint center( qreal radius ) 
        {
                return s.rect2_.center() + radius * ( s.rect1_.center() - s.rect2_.center() );
        }
        
};


class GWMouseEventDispatcher : public AbstractMouseEventDispatcher
{
public:
        GWMouseEventDispatcher( GradientWidget& gw ) : gw_( gw ) {}
        virtual bool initialClick( QMouseEvent* e );
        virtual bool finalClick( QMouseEvent* e );
        virtual void drag( QMouseEvent* e );
        virtual void move( QMouseEvent* e );

private:
        GradientWidget& gw_;
};


GradientWidget::GradientWidget( Gradient* gr, QWidget * parent )
        : QWidget( parent ),
          movedPoint_( 0 ),
          colorStopIndex_( -1 ),
          dummyUIHandler( new DummyUIHandler(*this) ),
          linearUIHandler( new LinearUIHandler(*this) ),
          radialUIHandler( new RadialUIHandler(*this) ),
          moveInAction_( false )
{
        setGradient( gr );

        QSize s( handleSize_,handleSize_ );
        rect1_.setSize( s );
        rect2_.setSize( s );

        setHandleRects();
        
        setMinimumHeight( 100 );
        setMouseTracking( true );

        mouseDispatcher_ = new GWMouseEventDispatcher( *this );
        mouseEventHandler_ = new ClickMouseEventHandler( mouseDispatcher_ );
}



GradientWidget::~GradientWidget()
{
        delete mouseDispatcher_;
}

void GradientWidget::setGradient( Gradient* gr )
{
        gradient_ = gr;
        if ( gr->type() != Gradient::None ) {
                GradientHandler grh( *gr );
                point1_ = grh.start();
                point2_ = grh.final();

                initUIHandler();
        }
        else
                uiHandler_ = dummyUIHandler;

        update();    
}

void GradientWidget::initUIHandler()
{
        if ( isEnabled() )
                switch ( gradient_->type() ) {
                    case Gradient::Linear: initLinearGradient(); break;
                    case Gradient::Radial: initRadialGradient(); break;
                    default: break;
                }
        else
                uiHandler_ = dummyUIHandler;
}


void GradientWidget::initLinearGradient()
{
        uiHandler_ = linearUIHandler;
}

void GradientWidget::initRadialGradient()
{
        uiHandler_ = radialUIHandler;
}

void GradientWidget::changeEvent( QEvent* e )
{
        if ( e->type() == QEvent::EnabledChange )
                initUIHandler();

        QWidget::changeEvent( e );
}

void GradientWidget::paintEvent( QPaintEvent* e )
{
        QPainter p( this );

        p.setClipRect( e->rect() );

        const int fw = 50;
        
        QRect r;
        r.setSize( QSize( fw,fw ) );

        bool inc = !( int(ceil(double(height())/fw))%2 );
        
        int i = 0;
        for ( int x=0; x<width(); x+=fw ) {
                for( int y=0; y<height(); y+=fw ) {
                        r.moveTopLeft( QPoint(x,y) );
                        QColor c( ( i++ % 2 ) ? Qt::lightGray : Qt::darkGray );
                        
                        p.fillRect( r, c );
                }
                if ( inc )
                        i++;
        }

        if ( gradient_->type() != Gradient::None ) {
                r = QRect(  handleSize_, handleSize_, size_.width(), size_.height() );
                p.setBrush( QBrush( *gradient_->toQGradient( r ) ) );
                p.drawRect( r );
                p.setBrush( Qt::NoBrush );

                p.setPen( Qt::white );
                p.drawRect( rect1_ );

                p.setPen( Qt::black );
                p.drawRect( rect2_ );

                uiHandler_->paint( &p );
        }
        
        p.end();
}

void GradientWidget::LinearUIHandler::paint( QPainter* p )
{        
        foreach ( QGradientStop cs, s.gradient_->colorStops() ) 
                p->drawEllipse( s.clsToRect( cs.first ) );
        
        p->drawLine( s.rect1_.center(), s.rect2_.center() );
}

void GradientWidget::RadialUIHandler::paint( QPainter* p ) 
{       
        qreal dia = 2*s.gradient_->radius()*hypot( s.size_.width(), s.size_.height() );
        QRectF rect = Geom::centerRect( s.rect1_.center(), QSizeF( dia, dia ) );

        
        p->setPen( Qt::DotLine );
        p->drawEllipse( rect );

        p->setPen( Qt::DashLine );
        foreach ( QGradientStop gs, s.gradient_->colorStops() ) {
                qreal d = dia*gs.first;
                rect = Geom::centerRect( center( gs.first ), QSizeF( d,d ) );
                p->drawEllipse( rect );
                p->drawRect( Geom::centerRect( center(gs.first), QSize(handleSize_,handleSize_)));
        }                      
}


void GradientWidget::resizeEvent( QResizeEvent* )
{
        size_.setWidth( width()-2*handleSize_ );
        size_.setHeight( height()-2*handleSize_ );
        
        update();
}

void GradientWidget::update()
{
        setHandleRects();
        
        QWidget::update();
}

QRect GradientWidget::clsToRect( double offset )
{
        QRect r;
        QPoint csp;
        r.setSize( QSize( handleSize_,handleSize_ ) );
        double t;
        t = ( point2_->x() - point1_->x() ) * offset;
        t += point1_->x();
        csp.setX( qRound( t * size_.width() ) + handleSize_ );
        t = ( point2_->y() - point1_->y() ) * offset;
        t += point1_->y();
        csp.setY( qRound( t * size_.height() ) + handleSize_ );
        r.moveCenter( csp );
        
        return r;
}

inline void GradientWidget::moveEdgePointTo( QPoint p )
{
        if ( p.x() > size_.width() )
                p.setX( size_.width() );
        else if ( p.x() < 0 )
                p.setX( 0 );
        
        if ( p.y() > size_.height() )
                p.setY( size_.height() );
        else if ( p.y() < 0 )
                p.setY( 0 );        

        double x = double(p.x()) / size_.width();
        double y = double(p.y()) / size_.height();        

        movedPoint_->setX( x );
        movedPoint_->setY( y );                
}

void GradientWidget::mouseReleaseEvent( QMouseEvent* e )
{
        if ( uiHandler_->isReal() )
                mouseEventHandler_->mouseReleaseEvent( e );
}

// void GradientWidget::mousePressEvent( QMouseEvent* e )
// {
//         mouseEventHandler_->mousePressEvent( e );
// }

void GradientWidget::mouseMoveEvent( QMouseEvent* e )
{
        if ( uiHandler_->isReal() )
                mouseEventHandler_->mouseMoveEvent( e );
}


bool GradientWidget::initialClick( QMouseEvent* e )
{
        const QPoint& p = e->pos();
        if ( rect1_.contains( p ) ) {
                movedPoint_ = point1_;
                moveInAction_ = true;
                return true;
        }
        
        if ( rect2_.contains( p ) ) {
                movedPoint_ = point2_;
                moveInAction_ = true;
                return true;
        }

        if ( e->modifiers() & Qt::ControlModifier ) {
                bool ok;
                QRgb rgb = QColorDialog::getRgba( Qt::black, &ok );
                if ( ok ) {
                        QColor c( rgb );
                        c.setAlpha( qAlpha( rgb ) ); 
                        gradient_->setColorAt( uiHandler_->calcOffset( p ), c );
                        update();
                        repaint();
                }
                return false;
        }

        if ( findColorStopUnderMouse( p, true ) ) {
                if ( e->modifiers() & Qt::ShiftModifier ) {
                        gradient_->colorStops().remove( colorStopIndex_ );
                        colorStopIndex_ = -1;
                        update();
                        repaint();
                        return false;
                }

                if ( e->modifiers() & Qt::AltModifier ) {
                        QColor& color = gradient_->colorStops()[colorStopIndex_].second;
                        bool ok;
                        QRgb rgb = QColorDialog::getRgba( color.rgba(), &ok );
                        if ( ok ) {
                                color = QColor( rgb );
                                color.setAlpha( qAlpha( rgb ) );
                                update();
                                repaint();
                        }
                        return false;
                }

                return true;
        }

        return uiHandler_->click( p );
}

bool GradientWidget::finalClick()
{
        movedPoint_ = 0;
        colorStopIndex_ = -1;

        moveInAction_ = false;
        setCursor( QCursor( Qt::ArrowCursor ) );

        return false;
}


void GradientWidget::drag( QMouseEvent* e )
{
        QPoint p = QPoint( e->pos().x() - handleSize_, e->pos().y() - handleSize_ );

        p.setX( qMax( p.x(), 0 ) );
        p.setY( qMax( p.y(), 0 ) );
        
        p.setX( qMin( p.x(), size_.width() ) );
        p.setY( qMin( p.y(), size_.height() ) );
        
        if ( movedPoint_ ) {
                moveEdgePointTo( p );
                update();
                repaint();
                return;
        }

        if ( colorStopIndex_ != -1 ) {
                gradient_->colorStops()[colorStopIndex_].first = uiHandler_->calcOffset( p );
                update();
                repaint();
                return;
        }
        
        uiHandler_->move( p );
}

void GradientWidget::move( QMouseEvent* e )
{
        const QPoint& p = e->pos();
                
        setCursor( QCursor( Qt::ArrowCursor ) );
        
        if ( findColorStopUnderMouse( p ) )
                setCursor( QCursor( Qt::CrossCursor ) );

        if ( rect1_.contains( p ) || rect2_.contains( p ) ) 
                setCursor( Qt::SizeAllCursor );

        RadialUIHandler* rh = dynamic_cast<RadialUIHandler*>( uiHandler_ );

        if ( rh && rh->radiusGrasped( p, rect1_.center(), gradient_->radius() ) )
                setCursor( Qt::SizeAllCursor );

}

bool GradientWidget::findColorStopUnderMouse( const QPoint& p, bool setIndex )
{
        int i = 0;
        foreach ( QGradientStop cs, gradient_->colorStops() ) {                
                if ( uiHandler_->colorStopHit( cs, p ) ) {
                        if ( setIndex )
                                colorStopIndex_ = i;
                        return true;
                }
                i++;
        }

        if ( setIndex )
                colorStopIndex_ = -1;
        return 0;
}

bool GradientWidget::LinearUIHandler::colorStopHit( const QGradientStop& gs, const QPoint& p )
{
        return s.clsToRect( gs.first ).contains( p );
}

bool GradientWidget::RadialUIHandler::colorStopHit( const QGradientStop& gs, const QPoint& p )
{
        return radiusGrasped( p, center( gs.first ), gs.first*s.gradient_->radius() );
}

bool GradientWidget::RadialUIHandler::click( const QPoint& p )
{        
        return radiusGrasped( p, s.rect1_.center(), s.gradient_->radius() );
}

double GradientWidget::LinearUIHandler::calcOffset( const QPoint& p )
{
        using namespace Geom;
        
        const QPointF a = QPointF( s.rect2_.center() - s.rect1_.center() );
        const QPointF b = QPointF( p - s.rect1_.center() );
        
        double a2 = scalarProduct( a,a );

        const QPointF px = scalarProduct(a,b)/a2 * a;
        
        return sqrt( scalarProduct(px,px)/a2 );
}

double GradientWidget::RadialUIHandler::calcOffset( const QPoint& p )
{
        using namespace Geom;

        const double r = s.gradient_->radius()*hypot( s.size_.width(), s.size_.height() );

        if ( distance( s.rect1_.center(), p ) > r )
                return 1;
        
        const QPointF F = s.rect1_.center() - s.rect2_.center();
        const QPointF X = p - s.rect2_.center();
        const double x = pabs( X );
        const double f = pabs( F );
        const double c = f * scalarProduct( F,X )/(f*x);
        
        double a = c + sqrt( c*c + r*r - f*f );

        return x / a;
}

void GradientWidget::RadialUIHandler::move( const QPoint& p )
{
                qreal rad = Geom::distance( p, s.rect1_.center() );
                rad /= hypot( s.size_.width(), s.size_.height() );
                s.gradient_->setRadius(rad);
                s.update();
//                s.repaint();
}


bool GradientWidget::RadialUIHandler::radiusGrasped( const QPoint& p, const QPoint& c,
                                                     qreal radius )
{                
        qreal d = Geom::distance( c, p );
        d -= radius * hypot( s.size_.width(), s.size_.height() );
        
        return ( fabs(d) < 3 );
}

void GradientWidget::setHandleRects()
{
        if ( gradient_->type() == Gradient::None )
                return;
        
        QPointF p = *point1_;
        p.rx() *= size_.width();
        p.ry() *= size_.height();
        rect1_.moveCenter( p.toPoint() + QPoint( handleSize_, handleSize_ ) );

        p = *point2_;
        p.rx() *= size_.width();
        p.ry() *= size_.height();
        rect2_.moveCenter( p.toPoint() + QPoint( handleSize_, handleSize_ ) );
}


void GradientWidget::contextMenuEvent( QContextMenuEvent* )
{
//         QMenu* ctMenu = new QMenu( this );
        
//         ctMenu->insertItem("Bla");
//         ctMenu->exec( QCursor::pos() );
//         delete ctMenu;
        
}

inline bool GWMouseEventDispatcher::initialClick( QMouseEvent* e )
{
        return gw_.initialClick( e );
}

inline bool GWMouseEventDispatcher::finalClick( QMouseEvent* )
{
        return gw_.finalClick();
}

inline void GWMouseEventDispatcher::move( QMouseEvent* e )
{
        gw_.move( e );
}

inline void GWMouseEventDispatcher::drag( QMouseEvent* e )
{
        gw_.drag( e );
}


