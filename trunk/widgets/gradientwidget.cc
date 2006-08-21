 
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

#include "gradientwidget.h"
#include "gradient.h"
#include "geometry.h"
#include "mouseeventhandler.h"
#include "paintutils.h"

#include <QPainter>
#include <QPaintEvent>
#include <QColorDialog>

#include <cmath>

#include <QDebug>

//! Abstract class of UIHandlers
/*! An UIHandler takes mouse clicks and mouse moves and calculates the
 *  what to do with the GradientWidget::gradient_.
 */
class GradientWidget::UIHandler
{
public:
        //! Takes a reference to the GradientWidget.
        UIHandler( GradientWidget* w ) : s( w ) {}

        virtual ~UIHandler() {}

        void setGradWidget( GradientWidget* w ) { s = w; }
        //! Called when the user clicks at a certain point.
        /*! Supposed to return true when a mouse dragging follows.
         */
        virtual bool click( const QPoint& ) { return false; }

        //! Called on mouse move
        virtual void move( const QPoint& ) {}

        //! supposed to paint the help lines and other items
        virtual void paint( QPainter* ) {}

        //! supposed to return true if the mouse cursor is near a color stop
        virtual bool colorStopHit( const QGradientStop&, const QPoint& ) { return false; }

        //! supposed to calculate the value for QColorStop represented by the point p
        virtual double calcOffset( const QPoint& ) { return -1; }

        //! is supposed to return true by subclasses.
        virtual bool isReal() const { return false; }
        
protected:
        //! the instance of the GradientWidget
        GradientWidget* s;
};

class GradientWidget::LinearUIHandler : public GradientWidget::UIHandler
{
public:
        LinearUIHandler( GradientWidget* w ) : UIHandler( w ) {}
        virtual void paint( QPainter* p );
        virtual bool colorStopHit( const QGradientStop& gs, const QPoint& p );
        virtual double calcOffset( const QPoint& p );
        virtual bool isReal() const { return true; }
};

class GradientWidget::RadialUIHandler : public GradientWidget::UIHandler
{
public:
        RadialUIHandler( GradientWidget* w )
                : UIHandler( w ) {}
        virtual bool click( const QPoint& );
        virtual void move( const QPoint& );
        virtual void paint( QPainter* p );
        virtual bool colorStopHit( const QGradientStop& gs, const QPoint& p );
        virtual double calcOffset( const QPoint& p );
        virtual bool isReal() const { return true; }

        //! returns true if p is on the edge of circle of a radius around c.
        bool radiusGrasped( const QPoint& p, const QPoint& c, qreal radius );

private:
        QPoint center( qreal offset ) 
        {
                return s->p2() + offset * ( s->p1() - s->p2() );
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
          colorStopIndex_( -1 )
{
        static UIHandler dh( this );
        dummyUIHandler = &dh;
        
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

/*! Therefore it makes point1_ and point2_ point to the appropriate
 *  points. Finally it has the uiHandler_ initialized, and the widget
 *  updated.
 */
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
        static LinearUIHandler h( this );
        h.setGradWidget( this );
        uiHandler_ =  &h;
}

void GradientWidget::initRadialGradient()
{
        static RadialUIHandler h( this );
        h.setGradWidget( this );
        uiHandler_ = &h;
}

//! handles enabling and disabeling of the widget
/*! Basically it makes uiHandler_ point to dummyUIHandler if the
 *  widget is disabled.
 */
void GradientWidget::changeEvent( QEvent* e )
{
        if ( e->type() == QEvent::EnabledChange )
                initUIHandler();

        QWidget::changeEvent( e );
}

//! repaints the widget
/*! paints the background chessboard, and then the Gradient. The two
 *  point defining the gradient are drawn. Finally it passes the
 *  QPainter to UIHandler::paint() so that the handles are drawn.
 */
void GradientWidget::paintEvent( QPaintEvent* e )
{
        QPainter p( this );

        p.setClipRect( e->rect() );

        PaintUtils::paintChessBoard( &p, QRect( QPoint(), size() ) );

        if ( gradient_->type() != Gradient::None ) {
                QRect r(  handleSize_, handleSize_, size_.width(), size_.height() );
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

//! paints the handles for the LinearUIHandler
/*! Draws a line from the start to the end and puts the handles for
 *  the color stops on it.
 */
void GradientWidget::LinearUIHandler::paint( QPainter* p )
{
        foreach ( QGradientStop cs, s->gradient_->colorStops() ) 
                p->drawEllipse( s->clsToRect( cs.first ) );
        
        p->drawLine( s->p1(), s->p2() );
}

void GradientWidget::RadialUIHandler::paint( QPainter* p ) 
{       
        qreal dia = 2*s->gradient_->radius()*hypot( s->size_.width(), s->size_.height() );
        QRectF rect = Geom::centerRect( s->p1(), QSizeF( dia, dia ) );

        
        p->setPen( Qt::DotLine );
        p->drawEllipse( rect );

        p->setPen( Qt::DashLine );
        foreach ( QGradientStop gs, s->gradient_->colorStops() ) {
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
        QPoint csp;
        double t;
        
        t = ( point2_->x() - point1_->x() ) * offset;
        t += point1_->x();
        csp.setX( qRound( t * size_.width() ) + handleSize_ );
        t = ( point2_->y() - point1_->y() ) * offset;
        t += point1_->y();
        csp.setY( qRound( t * size_.height() ) + handleSize_ );
        
        return Geom::centerRect( csp, QSize( handleSize_,handleSize_ ) );
}

/*! Note that the validity of movedPoint_ is not checked. Declared
 *  inline as there is only called from one point.
 */
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

        if ( ! ( e->modifiers() & Qt::AltModifier ) ) {

                if ( rect1_.contains( p ) ) {
                        movedPoint_ = point1_;
                        return true;
                }
        
                if ( rect2_.contains( p ) ) {
                        movedPoint_ = point2_;
                        return true;
                }
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

        if ( !(e->modifiers() & Qt::AltModifier) && (rect1_.contains( p ) || rect2_.contains( p )) ) 
                setCursor( Qt::SizeAllCursor );

        RadialUIHandler* rh = dynamic_cast<RadialUIHandler*>( uiHandler_ );

        if ( rh && rh->radiusGrasped( p, p1(), gradient_->radius() ) )
                setCursor( Qt::SizeAllCursor );

}

/*! If setindex is true, colorStopIndex_ is set to the index of the
 *  color stop found or to -1 if no color stop is found.
 */
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
        return s->clsToRect( gs.first ).contains( p );
}

bool GradientWidget::RadialUIHandler::colorStopHit( const QGradientStop& gs, const QPoint& p )
{
        return radiusGrasped( p, center( gs.first ), gs.first*s->gradient_->radius() );
}

bool GradientWidget::RadialUIHandler::click( const QPoint& p )
{        
        return radiusGrasped( p, s->p1(), s->gradient_->radius() );
}

double GradientWidget::LinearUIHandler::calcOffset( const QPoint& P )
{
        using namespace Geom;
        
        const QPointF A = QPointF( s->p2() - s->p1() );
        const QPointF B = QPointF( P - s->p1() );
        
        return scalarProduct( A,B ) / scalarProduct( A,A );
}


/*! The value to be calculated is the racio between the distances:
 *      - between the focal point \f$P_2\f$ and the point \f$P\f$: \f$x\f$
 *      - between the focal point and the point of intersection
 *        of the gradient circle and the line containing focal point and p,
 *        called A.
 *        
 *  Now the maths used:
 *  
 *  We consider the triangular of \f$P_2\f$, \f$A\f$ and \f$C\f$ where
 *  \f$C\f$ is the center of the gradient circle. Let \f$\alpha\f$ be
 *  the angle between \f$P_2 P\f$ and \f$P_2 C\f$ and the distances 
 *
 *  The cosinus theorem gives us the following relation:
 *  \f[
 *      r^2 = f^2 + a^2 - 2 a f \cos\alpha
 *  \f]
 *  
 *  where \f$r\f$ is Gradient::radius_, \f$a\f$ is the distance \f$P_2
 *  A\f$ and \f$f\f$ the distance \f$P_2 C\f$. Now let's the
 *  expression \f$f \cos\alpha\f$ call \f$c\f$.
 *
 *  Using
 *  \f[
 *      <F,X> = f x \cos\alpha
 *  \f]
 *  where X is the vector \f$P_2 P\f$ we get by the cosinus theorem:
 *  \f[
 *      a = c + \sqrt{ c^2 + r^2 - f^2 }
 *  \f]
 *
 *  then we return a/x.
 *
 *  If p is in the gradient circle the result will be 1. As a
 *  colorstop can't have an offset > 1 we return 1 in case P is
 *  outside the gradient's circle.
 */
double GradientWidget::RadialUIHandler::calcOffset( const QPoint& P )
{
        using namespace Geom;

        const double r = s->gradient_->radius()*hypot( s->size_.width(), s->size_.height() );

        if ( distance( s->p1(), P ) > r )
                return 1;
        
        const QPointF F = s->p1() - s->p2();
        const QPointF X = P - s->p2();
        const double x = pabs( X );
        const double f = pabs( F );
        const double c = f * scalarProduct( F,X )/(f*x);
        
        double a = c + sqrt( c*c + r*r - f*f );

        return x / a;
}

void GradientWidget::RadialUIHandler::move( const QPoint& p )
{
                qreal rad = Geom::distance( p, s->p1() );
                rad /= hypot( s->size_.width(), s->size_.height() );
                s->gradient_->setRadius(rad);
                s->update();
//                s->repaint();
}


bool GradientWidget::RadialUIHandler::radiusGrasped( const QPoint& p, const QPoint& c,
                                                     qreal radius )
{                
        qreal d = Geom::distance( c, p );
        d -= radius * hypot( s->size_.width(), s->size_.height() );
        
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


