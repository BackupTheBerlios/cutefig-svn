 
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

#include "canvasview.h"

#include <QtGui>
#include <QtAlgorithms>

#include <cmath>

#include "fig.h"
#include "geometry.h"
#include "allobjects.h"
#include "cutefig.h"
#include "ruler.h"
#include "addcommand.h"
#include "deletecommand.h"
#include "changecommand.h"
#include "actions.h"
#include "interactiveaction.h"

/** \class CanvasView
 *
 * This class is responsible to display the figure to the user and to
 * handle user interaction. It accepts key and mouse events, figures
 * out whether they are to be handled and tells the Controler to call
 * InteractiveAction::click() or InteractiveAction::move()
 * respectively.
 *
 * The second job of CanvasView is to pass a QPainter to the Figure to
 * which the DrawObjects can be painted to. Furthermore it draws the
 * paper and the snap grid. It communicates with the two instances of
 * Ruler to tell them about mouse moves and changes of the
 * zoomscale. Drawing the helplines is also planned.
*/

//! calculates the Fig::PointFlag out of m and b. 
Fig::PointFlag calcPointFlag( Qt::MouseButtons b, Qt::KeyboardModifiers m );

qreal CanvasView::clickTolerance_ = 5;

/**
 * The constructor needs a Controler and a figure as parameter.
 */
CanvasView::CanvasView( Controler* c, Figure* f,  CuteFig* parent )
        : QWidget( parent ),
          ViewBase( c, f ),
          mainWindow_( parent ),
          unit_( Fig::cm2pix ),
          buffer_( 1000,1000 ),
          oldRect_(),
          oldSnapPoint_( QPoint(0,0) ),
          snapped_( false ),
          tentativeDraw_( false )
{
        QSize size( 500, 500 );
        setFocusPolicy( Qt::StrongFocus );
    
        setFocusPolicy( Qt::StrongFocus );
        offset_ = QPoint( 0,0 );
        setAttribute( Qt::WA_NoBackground );

        setMouseTracking( true );
        setFixedSize( size );

        scale_ = 1.0;
        setZoom_private( 3.0 );
        setGrid( 1.0 );
        setSnap( 1.0 );

        updateFigure();

//         activeToolActions_.append( controler_->toolActions()->move() );
//         activeToolActions_.append( controler_->toolActions()->scale() );
}

void CanvasView::mouseDoubleClickEvent( QMouseEvent* e )
{
        return;
        
        mouseReleaseEvent( e );
}

/**  
 */
void CanvasView::mouseReleaseEvent( QMouseEvent* e )
{
        Fig::PointFlag f = calcPointFlag( e->button(), e->modifiers() );

        snapped_ = false;

        if ( f & Fig::Cancel ) {
                controler_->cancelAction();
                return;
        }

        DrawObject* o = 0;

        QPoint p = e->pos();

        if ( controler_->selectedObjects().isEmpty() || 
             e->modifiers() & Qt::ControlModifier &&
             ! controler_->hasAction() ) {
                o = figure_->findContainingObject( p * scaleMatrixInv_, clickTolerance_ );
                controler_->selectObject( o );
        }

        if ( controler_->willAcceptAction( p, &scaleMatrixInv_ ) ) {
                snap( p );
                setCursor( controler_->callActionClick( p, f, &scaleMatrixInv_ ) );
        } else
                if ( !o ) {
                        controler_->cancelAction();
                        controler_->selectObject( o );
                }
}


/** 
 */
void CanvasView::mouseMoveEvent( QMouseEvent* e )
{
        QPoint p = e->pos();
        hRuler_->setValue( p.x() );
        vRuler_->setValue( p.y() );

        QString s;
        QTextStream st( &s );
        st.setRealNumberPrecision( 2 );
        st.setRealNumberNotation( QTextStream::FixedNotation );
        st << Fig::pix2cm*p.x()/scale_ << " : " << Fig::pix2cm*p.y()/scale_
           << "  -  "
           << p.x() << " : " << p.y();
        mainWindow_->statusBar()->showMessage( s );

        if ( controler_->wouldAcceptAction( p, &scaleMatrixInv_ ) ) {
                if ( snap( p ) ) 
                        controler_->callActionMove( p, &scaleMatrixInv_ );
        } else {
                QCursor crs;
                DrawObject* o = 0;
                if (controler_->selectedObjects().isEmpty()||e->modifiers()&Qt::ControlModifier) {
                        o = figure_->findContainingObject( p * scaleMatrixInv_, clickTolerance_ );
                        crs = controler_->considerObject( o, p, &scaleMatrixInv_ );
                } else
                        crs = controler_->findToolAction( p, &scaleMatrixInv_ );
                setCursor( crs );
        }
}


void CanvasView::contextMenuEvent( QContextMenuEvent* e )
{
        if ( controler_->wouldAcceptAction( e->pos(), &scaleMatrixInv_ ) ) {
                e->ignore();
                return;
        }

        QMenu* cm = new QMenu( this );
        QListIterator<ActionCollection*> it( controler_->actionGroups() );
        
        while ( it.hasNext() ) {
                foreach ( QAction* a, it.next()->actions() )
                        if ( a->isEnabled() ) {
                                InteractiveAction* ia = qobject_cast<InteractiveAction*>( a );
                                if ( ia ) {
                                        if ( ia->wouldHandleSelection() )
                                                cm->addAction( ia );
                                } else
                                        cm->addAction( a );
                        }
                if ( it.hasNext() )
                        cm->addSeparator();
        }

        int i = 0;
        foreach ( QAction* a, cm->actions() )
                if ( !a->isSeparator() )
                        ++i;

        if ( i )
                cm->popup( e->globalPos() );
        else
                e->ignore();
}

void CanvasView::keyReleaseEvent( QKeyEvent* e )
{
        switch ( e->key() ) {
            case Qt::Key_Escape: 
                    controler_->cancelAction();
                    controler_->selectObject( 0 );
                    snapped_ = false;
                    break;
            case Qt::Key_Return:
                    handleReturnHit();
            default:
                    e->ignore();
        }
}

inline void CanvasView::handleReturnHit()
{
        QPoint p; 
        if ( snapped_ )
                p =  oldSnapPoint_;
        else 
                p = mapFromGlobal( QCursor::pos() );
            
        if ( controler_->willAcceptAction( p, &scaleMatrixInv_ ) ) 
                controler_->callActionClick( p, Fig::Final, &scaleMatrixInv_ );
}

// Drawing routines

// draws the toolobject. Therefore it does not use the toolObject pen and 
// brush but a default pen
//

void CanvasView::drawSelection( QPainter* p )
{
        QRectF dr;
        p->setPen( Qt::blue );
        dr.setSize( QSize( 5,5 ) );

        QRectF r;
        foreach ( DrawObject* o, controler_->selection().objects() ) {
                r = o->boundingRect();
                r = scaleMatrix_.mapRect( r );

                foreach ( QPointF pt, Geom::boundingPoints( r ) ) 
                        if ( !pt.isNull() ) {
                                dr.moveCenter( pt );
                                p->fillRect( dr, Qt::blue );
                        }
        }
}

// redraws the whole figure
//
void CanvasView::updateFigure( const Selection& s, bool tentative )
{        
        if ( !tentative ) {
                QPainter p;
                p.begin( &buffer_ );
                p.setRenderHint( QPainter::Antialiasing, true );
                drawPaper( &p );
                drawGrid( &p );
                p.setMatrix( scaleMatrix_ );
                figure_->drawElements( &p, s.backups() );
                p.end();
        }

        tentativeDraw_ = tentative;

        if ( !s.isEmpty() ) {
                QRect r = scaleMatrix_.mapRect( s.boundingRect().toRect() );
                repaint();// r | oldRect_ );
                oldRect_ = r;
        }
        else {
                update();
                oldRect_ = QRect();
        }
        tentativeDraw_ = false;
}

void CanvasView::paintEvent( QPaintEvent * e )
{
        QPainter p;
        p.begin( this );
        p.setRenderHint( QPainter::Antialiasing, true );
        p.setClipRect( e->rect() );
        p.drawPixmap( e->rect(), buffer_, e->rect() );

        p.setMatrix( scaleMatrix_ );
        const ObjectList& l = controler_->selectedObjects();

        if ( tentativeDraw_ )
                foreach ( DrawObject* o, l )
                        o->drawTentative( &p );
        else
                foreach ( DrawObject* o, l )
                        o->draw( &p );

        p.resetMatrix();

        drawObjectsPoints( &p );
        drawSelection( &p );

        if ( snapped_ )
                drawSnap( &p );
 
        p.end();
}

inline void CanvasView::drawObjectsPoints( QPainter* p )
{
        const QPolygonF pa = controler_->objectsPoints();
        if ( pa.empty() )
                return;

        QRectF r;
        r.setSize( QSize( 4,4 ) );
        
        p->setPen( QPen( Qt::SolidPattern, 1.0 ) );
        QPolygonF::const_iterator it = pa.begin();
        for ( ; it != pa.end(); ++it )
                if ( ! it->isNull() ) {
                        r.moveCenter( (*it*scaleMatrix_) );
                        p->drawRect( r );
                }
}


// draws the background of the figure
//
inline void CanvasView::drawPaper( QPainter* p )
{
        QRect r( offset_.x(), offset_.y(), width()-offset_.x(), height()-offset_.y() );
        p->fillRect( r, Qt::white ); // makes lineWidth spinbox in ObjecDialog crazy, why?    
}

// draws the grid
//
inline void CanvasView::drawGrid( QPainter* p )
{
        if ( !gridScaled_ )
                return;

        for ( qreal x=0; x<buffer_.width(); x+=gridScaled_ )
                for ( qreal y=0; y<buffer_.height(); y+=gridScaled_ )
                        p->drawPoint( QPointF( x, y ) );
}

// emphasises the point the mouse position has been snapped to
//
inline void CanvasView::drawSnap( QPainter* pt )
{
        QRectF r;
        r.setSize( QSizeF( 9,9 ) );
        r.moveCenter( snapPoint_ );

        pt->drawEllipse( r );
}


// zooming, resizing etc...

void CanvasView::zoomIn()
{
        setZoom( zoom_ + 0.1 );
}

void CanvasView::zoomOut()
{
        if ( zoom_ > 0.2 )
                setZoom( zoom_ - 0.1 );
}

void CanvasView::zoomFit()
{
        QRectF r = figure_->boundingRect();
        if ( r.isNull() )
                return;

        QSizeF vs = mainWindow_->viewportSize();
        QSizeF fs = r.size();

        setZoom( qMin( vs.width()/fs.width(), vs.height()/fs.height() ) );
}

void CanvasView::zoomOrig()
{
        setZoom( 1. );
}

void CanvasView::setZoom( double z )
{
        setZoom_private( z );
        updateFigure();
}

void CanvasView::setZoom_private( double z )
{
        zoom_ = z;
        double scaleRatio = 1/scale_;
        scale_ = zoom_ * figure_->scale();
        scaleRatio *= scale_;
        setFixedSize( size() * scaleRatio );
        scaleMatrix_.scale( scaleRatio, scaleRatio );
        scaleMatrixInv_ = scaleMatrix_.inverted();
        calcGrid();
        emit( scaleChanged( scale_ ) );
}

void CanvasView::setFixedSize( const QSize &s )
{
        buffer_ = QPixmap( s );
        buffer_.fill( Qt::white );
        resize( s );
}

void CanvasView::setGrid( double gridWidth )
{
        gridWidth_ = unit_ * gridWidth;
        calcGrid();
}

void CanvasView::setSnap( double snapWidth )
{
        snapWidth_ = unit_ * snapWidth;
        calcGrid();
}

void CanvasView::calcGrid()
{
        gridScaled_ = gridWidth_ * zoom_;
        if ( gridScaled_ < 5 )
                gridScaled_ = -1.0;

        snapScaled_ = snapWidth_ * zoom_;
        if ( snapScaled_ < 5 )
                snapScaled_ = -1.0;
}

// snaps a point to the snapgrid (parameter will be modified)
//
bool CanvasView::snap( QPoint & p )
{
        if ( !snapScaled_ )
                return true;

        p.setX( qRound( qRound(qreal(p.x())/snapScaled_) * snapScaled_ ) );
        p.setY( qRound( qRound(qreal(p.y())/snapScaled_) * snapScaled_ ) );

        bool snapped = ( p != oldSnapPoint_ );
        
        snapPoint_ = p;

        if ( snapped ) {
                oldSnapPoint_ = snapPoint_;
                snapped_ = true;
                repaint();
        }

        return snapped;
}

Fig::PointFlag calcPointFlag( Qt::MouseButtons b, Qt::KeyboardModifiers m )
{
        Fig::PointFlag pf = Fig::Normal;
        if ( b & Qt::RightButton )
                pf = Fig::PointFlag( pf | Fig::Cancel );
        if ( b & Qt::MidButton )
                pf = Fig::PointFlag( pf | Fig::Final );

        if ( m & Qt::ShiftModifier )
                pf = Fig::PointFlag( pf | Fig::Straight );
        if ( m & Qt::ControlModifier )
                pf = Fig::PointFlag( pf | Fig::Special1 );
        if ( m & Qt::AltModifier )
                pf = Fig::PointFlag( pf | Fig::Special2 );

        return pf;
}
