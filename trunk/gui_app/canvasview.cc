 
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
#include "drawobject.h"
#include "figure.h"
#include "cutefig.h"
#include "ruler.h"
#include "actions.h"
#include "interactiveaction.h"
#include "griddialog.h"

/** \class CanvasView
 *
 * This class is responsible to display the figure to the user and to
 * handle user interaction. It accepts key and mouse events, figures
 * out whether they are to be handled and tells the Controler to call
 * InteractiveAction::click(), InteractiveAction::move() or
 * InteractiveAction::keyStroke() respectively.
 *
 * The second job of CanvasView is to pass a QPainter to the Figure to
 * which the DrawObjects can be painted to. Furthermore it draws the
 * paper and the snap grid. It communicates with the two instances of
 * Ruler to tell them about mouse moves and changes of the
 * zoomscale. Drawing the helplines is also planned.
 */

//! calculates the Fig::PointFlag out of m and b. 
Fig::PointFlag calcPointFlag( Qt::MouseButtons b, Qt::KeyboardModifiers m );

/**
 * The constructor needs a Controler and a figure as parameter.
 */
CanvasView::CanvasView( Controler* c, Figure* f,  CuteFig* parent )
        : QWidget( parent ),
          ViewBase( c, f ),
          mainWindow_( parent ),
          scale_( 1 ),
          hRuler_( 0 ),
          vRuler_( 0 ),
          unit_( Fig::cm2pix ),
          oldRect_(),
          oldSnapPoint_( QPoint(0,0) ),
          snapped_( false ),
          tentativeDraw_( false )
{
        setFocusPolicy( Qt::StrongFocus );
        offset_ = QPoint( 0,0 );
//        setAttribute( Qt::WA_NoBackground );
        setAttribute( Qt::WA_InputMethodEnabled );
        setAttribute( Qt::WA_KeyCompression );
        
        setMouseTracking( true );
        doResizing();
        setGridWidth( .5 );
        setSnapWidth( 1.0 );

//        updateFigure();
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
                o = figure_->findContainingObject( p * scaleMatrixInv_ );
                controler_->selectObject( o );
        }

        if ( controler_->willAcceptClick( p, &scaleMatrixInv_ ) ) {
                snap( p );
                setCursor( controler_->callActionClick( p, f, &scaleMatrixInv_ ) );
        } else
                if ( !o ) {
                        controler_->cancelAction();
                        controler_->selectObject( 0 );
                }
}


/** 
 */
void CanvasView::mouseMoveEvent( QMouseEvent* e )
{
        QPoint p = e->pos();
        if ( hRuler_ )
                hRuler_->setValue( p.x() );
        if ( vRuler_ )
                vRuler_->setValue( p.y() );

        QPointF pf( QPointF( p ) * scaleMatrixInv_ );
        
        QString s;
        QTextStream st( &s );
        st.setRealNumberPrecision( 2 );
        st.setRealNumberNotation( QTextStream::FixedNotation );
        st << pf.x()/unit_ << " : " << pf.y()/unit_ << "  -  " << p.x() << " : " << p.y();
        mainWindow_->statusBar()->showMessage( s );

        if ( controler_->wouldAcceptClick( p, &scaleMatrixInv_ ) ) {
                if ( snap( p ) ) 
                        controler_->callActionMove( p, &scaleMatrixInv_ );
        } else {
                QCursor crs;
                DrawObject* o = 0;
                if (controler_->selectedObjects().isEmpty()||e->modifiers()&Qt::ControlModifier) {
                        o = figure_->findContainingObject( p * scaleMatrixInv_ );
                        crs = controler_->considerObject( o, p, &scaleMatrixInv_ );
                } else
                        crs = controler_->findToolAction( p, &scaleMatrixInv_ );
                setCursor( crs );
        }
}


void CanvasView::contextMenuEvent( QContextMenuEvent* e )
{
        if ( controler_->wouldAcceptClick( e->pos(), &scaleMatrixInv_ ) ) {
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

void CanvasView::keyPressEvent( QKeyEvent* e )
{
        if ( controler_->callActionKeyStroke( e ) ) {
                e->accept();
                return;
        }
        
        switch ( e->key() ) {
            case Qt::Key_Escape: 
                    controler_->cancelAction();
                    controler_->selectObject( 0 );
                    snapped_ = false;
                    break;
            case Qt::Key_Return:
                    handleReturnHit();
            default:
                    break;
        }
        e->accept();
}

bool CanvasView::event( QEvent* e )
{
        if ( e->type() == QEvent::ShortcutOverride ) {
                QKeyEvent* ke = (QKeyEvent*) e;
                if ( controler_->willAcceptKeyStroke() &&
                     ke->modifiers() <= int(Qt::ShiftModifier) )
                        ke->accept();
        }

        if ( e->type() == QEvent::User ) {
                updateFigureImediately();
                return true;
        }

        return QWidget::event( e );
}

void CanvasView::inputMethodEvent( QInputMethodEvent* e )
{
        if ( controler_->callInputMethodHandler( e ) ) 
                e->accept();
        else
                e->ignore();
}

void CanvasView::enterEvent( QEvent* )
{
        if ( hRuler_ )
                hRuler_->setIndicating( true );
        if ( vRuler_ )
                vRuler_->setIndicating( true );
}

void CanvasView::leaveEvent( QEvent* )
{
        if ( hRuler_ )
                hRuler_->setIndicating( false );
        if ( vRuler_ )
                vRuler_->setIndicating( false );
}

QVariant CanvasView::inputMethodQuery( Qt::InputMethodQuery q )
{
        return QWidget::inputMethodQuery( q );
}

inline void CanvasView::handleReturnHit()
{
        QPoint p; 
        if ( snapped_ )
                p =  oldSnapPoint_;
        else 
                p = mapFromGlobal( QCursor::pos() );
            
        if ( controler_->willAcceptClick( p, &scaleMatrixInv_ ) ) 
                controler_->callActionClick( p, Fig::Final, &scaleMatrixInv_ );
}

// Drawing routines

// draws the toolobject. Therefore it does not use the toolObject pen and 
// brush but a default pen
//

void CanvasView::drawSelection( QPainter* p ) const
{
        QRectF dr;
        dr.setSize( QSize( 5,5 ) );

        QRectF r;
        foreach ( DrawObject* o, controler_->selection().objects() ) {
                r = o->controlPointRect();
                r = scaleMatrix_.mapRect( r );

                foreach ( QPointF pt, Geom::boundingPoints( r ) ) 
                        if ( !pt.isNull() ) {
                                dr.moveCenter( pt );
                                p->fillRect( dr, Qt::blue );
                        }
        }
}

void CanvasView::updateFigure( bool tentative )
{
        tentativeDraw_ = tentative;
        QApplication::postEvent( this, new QEvent( QEvent::User ) );
}

// redraws the whole figure
//
void CanvasView::updateFigureImediately()
{        
        if ( !tentativeDraw_ ) {
                QPainter p;
                QPixmap b( size() );
                p.begin( &b );
                drawPaper( &p );
                drawGrid( &p );
                p.setRenderHint( QPainter::Antialiasing, true );
                p.setMatrix( scaleMatrix_ );
                figure_->drawElements( &p, controler_->backups() );
                p.end();

                QPalette pal;
                pal.setBrush( QPalette::Window, b );
                setPalette( pal );
        }

        if ( !controler_->selectedObjects().isEmpty() ) {
                QRect r = scaleMatrix_.mapRect( controler_->selection().boundingRect().toRect() );
                update();// r | oldRect_ );
                oldRect_ = r;
        }
        else {
                update();
                oldRect_ = QRect();
        }
}

void CanvasView::paintEvent( QPaintEvent * e )
{
        const ObjectList& l = controler_->selectedObjects();

        if ( l.isEmpty() && !snapped_ )
                return;
        
        QPainter p;
        p.begin( this );
        p.setRenderHint( QPainter::Antialiasing, true );
        p.setClipRect( e->rect() );

        p.setMatrix( scaleMatrix_ );

        if ( tentativeDraw_ ) 
                foreach ( DrawObject* o, l ) 
                        o->drawTentative( &p );
        else
                foreach ( DrawObject* o, l )
                        o->draw( &p );

        p.resetMatrix();

//        drawObjectsPoints( &p );
        controler_->drawActionMetaData( &p, this );
        
        if ( snapped_ ) {
                p.setPen( Qt::green );
                p.drawEllipse( snapRect() );
        }
        
        p.end();
}

void CanvasView::drawObjectsMetaData( QPainter* p, const DrawObject* o ) const
{
        p->setMatrix( scaleMatrix_ );
        o->drawMetaData( p );
        p->resetMatrix();
}

void CanvasView::drawObjectsPoints( QPainter* p, const DrawObject* o ) const
{
        const QPolygonF& pa = o->points();
        if ( pa.isEmpty() )
                return;

        QRectF r;
        r.setSize( QSize( 4,4 ) );
        
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
        p->fillRect( r, Qt::white ); // makes spinboxes in ObjectDialog crazy, why?
}

// draws the grid
//
inline void CanvasView::drawGrid( QPainter* p )
{
        if ( gridScaled_ < 0 )
                return;

        p->setPen( QPen( Qt::lightGray , 1, Qt::DotLine ) );
        
        for ( double x = 0; x < width(); x += gridScaled_ )
                p->drawLine( QPointF(x,0), QPointF(x,height()) );
        for ( double y = 0; y < height(); y += gridScaled_ )
                p->drawLine( QPointF(0,y), QPointF(width(),y) );
}

// emphasises the point the mouse position has been snapped to
//
QRect CanvasView::snapRect()
{
        QRect r;
        r.setSize( QSize( 9,9 ) );
        r.moveCenter( snapPoint_ );

        return r;
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
}

void CanvasView::newSnapGrid()
{
        double ogw = gridWidth_;
        double osw = snapWidth_;
        
        GridDialog dlg( gridWidth_, snapWidth_, this );
        if ( dlg.exec() == QDialog::Rejected ) {
                gridWidth_ = ogw;
                snapWidth_ = osw;
                calcGrid();        
        }
}

void CanvasView::refineGrid()
{
        gridWidth_ -= 0.5;
        snapWidth_ -= 0.5;
        calcGrid();
}

void CanvasView::corsenGrid()
{
        gridWidth_ += 0.5;
        snapWidth_ += 0.5;
        calcGrid();
}

void CanvasView::setZoom_private( double z )
{
        zoom_ = z;
        double scaleRatio = 1/scale_;
        scale_ = zoom_ * figure_->scale();
        scaleRatio *= scale_;
        doResizing();
        scaleMatrix_.scale( scaleRatio, scaleRatio );
        scaleMatrixInv_ = scaleMatrix_.inverted();
        calcGrid();
        emit scaleChanged( scale_ );

        if ( hRuler_ )
                hRuler_->setScale( scale_ );
        if ( vRuler_ )
                vRuler_->setScale( scale_ );
}

void CanvasView::doResizing()
{
        QSize s = ( figure_->paperSize() * scale_ * unit_ ).toSize();
        
        if ( hRuler_ )
                hRuler_->setLength( s.width() );
        if ( vRuler_ )
                vRuler_->setLength( s.height() );
        resize( s );    
}

double CanvasView::paperWidth() const
{
        return figure_->paperSize().width() * scale_;
}

void CanvasView::setHRuler( Ruler* r )
{
        hRuler_ = r;
        r->setLength( width() );
}

void CanvasView::setVRuler( Ruler* r )
{
        vRuler_ = r;
        r->setLength( height() );
}

void CanvasView::setGridWidth( double gridWidth )
{
        gridWidth_ = gridWidth;
        calcGrid();
}

void CanvasView::setSnapWidth( double snapWidth )
{
        snapWidth_ = snapWidth;
        calcGrid();
}

void CanvasView::calcGrid()
{
        gridScaled_ = gridWidth_ * unit_ * zoom_;
        if ( gridScaled_ < 5 )
                gridScaled_ = -1.0;

        snapScaled_ = snapWidth_ * unit_ * zoom_;
        if ( snapScaled_ < 5 )
                snapScaled_ = -1.0;

        updateFigure();
}

// snaps a point to the snapgrid (parameter will be modified)
//
bool CanvasView::snap( QPoint & p )
{
        if ( ! ( snapScaled_ && controler_->actionWantsSnap( p, &scaleMatrixInv_ ) ) ) {
                if ( snapped_ )
                        update();
                snapped_ = false;
                return true;
        }

        p.setX( qRound( qRound(qreal(p.x())/snapScaled_) * snapScaled_ ) );
        p.setY( qRound( qRound(qreal(p.y())/snapScaled_) * snapScaled_ ) );

        bool snapped = ( p != oldSnapPoint_ );
        
        snapPoint_ = p;

        if ( snapped ) {
                oldSnapPoint_ = snapPoint_;
                snapped_ = true;
                update();
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



