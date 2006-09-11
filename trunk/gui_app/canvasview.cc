 
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
#include "statuswidgets.h"
#include "pointflagscalc.h"



/**
 * The constructor needs a Controler and a figure as parameter.
 */
CanvasView::CanvasView( Controler* c, ActionStatusIndicator* si, const Figure* f, CuteFig* parent )
        : QWidget( parent ),
          ViewBase( c, f ),
          mainWindow_( parent ),
          hRuler_( 0 ),
          vRuler_( 0 ),
          scale_( 1 ),
          unit_( f->unit() ),
          paperSize_( f->paper().size() ),
          oldRegion_(),
          oldSnapPoint_( QPoint(0,0) ),
          snapped_( false ),
          tentativeDraw_( false ),
          statusIndicator_( si )
{
        setFocusPolicy( Qt::StrongFocus );
        offset_ = QPoint( 0,0 );
        setAttribute( Qt::WA_InputMethodEnabled );
        setAttribute( Qt::WA_KeyCompression );
        setAttribute( Qt::WA_NoSystemBackground );
        setAutoFillBackground( false );
        setMouseTracking( true );
        doResizing();
        setGridWidth( .5 );
        setSnapWidth( 1.0 );
        qApp->installEventFilter( this );
        setupStatus();

        connect( controler_, SIGNAL( actionStatusChanged(const ActionStatus&) ),
                 si, SLOT( showStatus(const ActionStatus&) ) );
        connect( this, SIGNAL( status(const ActionStatus&) ),
                 si, SLOT( showStatus(const ActionStatus&) ) );
        connect( controler_, SIGNAL( actionIsAway(bool) ),
                 this, SLOT( takeOverStatusIndicator(bool) ) );
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
        Fig::PointFlags f = PointFlagsCalc::calcPointFlags( e->button(), e->modifiers() );

        snapped_ = false;

        if ( f & Fig::Cancel ) {
                controler_->cancelAction();
                return;
        }

        DrawObject* o = 0;

        QPoint p = e->pos();

        if ( controler_->selectedObjects().isEmpty() || 
             e->modifiers() & Qt::ControlModifier    &&
             !controler_->hasAction() ) {
                o = figure_->findContainingObject( p * scaleMatrixInv_ );
                controler_->selectObject( o );
        }

        if ( controler_->willAcceptClick( p, scaleMatrixInv_ ) ) {
                snap( p );
                setCursor( controler_->callActionClick( p, f, scaleMatrixInv_ ) );
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

        emit cursorMovedTo( p );
        
        if ( controler_->wouldAcceptClick( p, scaleMatrixInv_ ) ) {
                if ( snap( p ) ) 
                        controler_->callActionMove( p, scaleMatrixInv_ );
        } else {
                QCursor crs;
                DrawObject* o = 0;
                if (controler_->selectedObjects().isEmpty()||e->modifiers()&Qt::ControlModifier) {
                        o = figure_->findContainingObject( p * scaleMatrixInv_ );
                        crs = controler_->considerObject( o, p, scaleMatrixInv_ );
                } else
                        crs = controler_->findToolAction( p, scaleMatrixInv_ );
                setCursor( crs );
        }
}


void CanvasView::contextMenuEvent( QContextMenuEvent* e )
{
        if ( controler_->wouldAcceptClick( e->pos(), scaleMatrixInv_ ) ) {
                e->ignore();
                return;
        }

        QMenu* cm = new QMenu( this );
        QListIterator<ActionCollection*> it( controler_->actionGroups() );
        
        while ( it.hasNext() ) {
                foreach ( QAction* a, it.next()->actions() )
//                         if ( a->isEnabled() ) {
//                                 InteractiveAction* ia = qobject_cast<InteractiveAction*>( a );
//                                 if ( ia ) {
//                                         if ( ia->wouldHandleSelection() )
//                                                 cm->addAction( ia );
//                                 } else
//                                         cm->addAction( a );
//                         }
                        cm->addAction( a );
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
        dispatchModifierChange( e );
        
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

void CanvasView::keyReleaseEvent( QKeyEvent* e )
{
        dispatchModifierChange( e );
}


void CanvasView::dispatchModifierChange( const QKeyEvent* e )
{
        if ( e->modifiers() != status_.modifiers() ) {
                status_.setModifiers( e->modifiers() );
                if ( controler_->hasAction() )
                        controler_->modifierChange( e->modifiers() );
                else
                        emit status( status_ );
        }
}

bool CanvasView::event( QEvent* e )
{
        if ( e->type() == QEvent::ShortcutOverride ) {
                QKeyEvent* ke = static_cast<QKeyEvent*>( e );
                if ( controler_->willAcceptKeyStroke() &&
                     ke->modifiers() <= int(Qt::ShiftModifier) )
                        ke->accept();
        }

        return QWidget::event( e );
}

/** As QMenuBar snatches away the focus, when the user hits the
 *  Alt-key we sometimes need to be faster and snatch the
 *  corresponding QKeyEvent before. This is done by installing this
 *  into the eventfilter list of QApplication. Then we snatch away the
 *  event in this function.
 */
bool CanvasView::eventFilter( QObject*, QEvent* e )
{
        if ( !controler_->actionIsActive() )
                return false;
        
        switch ( e->type() ) {
            case QEvent::KeyPress:
            case QEvent::KeyRelease:
            case QEvent::ShortcutOverride:
            {
                    QKeyEvent* kev = static_cast<QKeyEvent*>( e );
                    if (kev->key() == Qt::Key_Alt || kev->key() == Qt::Key_Meta) {
                            event( kev );
                            return true;
                    }
            }
            default: break;
        }        
        
        return false;
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
        emit cursorIsIn( true );
}

void CanvasView::leaveEvent( QEvent* )
{
        emit cursorIsIn( false );
}

QVariant CanvasView::inputMethodQuery( Qt::InputMethodQuery q )
{
        return QWidget::inputMethodQuery( q );
}

inline void CanvasView::handleReturnHit()
{
        QPoint p; 
        if ( snapped_ )
                p = oldSnapPoint_;
        else 
                p = mapFromGlobal( QCursor::pos() );
            
        if ( controler_->willAcceptClick( p, scaleMatrixInv_ ) ) 
                controler_->callActionClick( p, Fig::Final, scaleMatrixInv_ );
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

        QRegion clipRegion = drawingRegion();

        if ( snapped_ ) 
                clipRegion |= snapRect();

        clipRegion |= oldRegion_;
        oldRegion_ = clipRegion;
                
        if ( clipRegion.isEmpty()  )
                update();
        else
                update( clipRegion );
}


QRegion CanvasView::drawingRegion() const
{
        QRegion region;
        
        foreach ( DrawObject* o, controler_->selection().objects() ) {
                QRect r = scaleMatrix_.mapRect( o->controlPointRect() ).toRect();
                r.setLeft( r.left()-3 );
                r.setRight( r.right()+3 );
                r.setTop( r.top()-3 );
                r.setBottom( r.bottom()+3 );

                region |= r;
        }

        return region;
}


void CanvasView::paintEvent( QPaintEvent * e )
{
        const ObjectList& l = controler_->selectedObjects();
        
        QPainter p;
        p.begin( this );
        
        p.setClipRegion( e->region() );

#ifndef QT_NO_DEBUG_OUTPUT
        QTime stopWatch;
        stopWatch.start();
#endif

        drawPaper(&p);
        drawGrid(&p);

        
#ifndef QT_NO_DEBUG_OUTPUT
        int papgrid = stopWatch.elapsed();
#endif
        
        p.setRenderHint( QPainter::Antialiasing, true );
        p.setMatrix( scaleMatrix_ );

        foreach( const DrawObject* o, figure_->objectsToBeDrawn() ) 
                if ( !controler_->backups().contains( const_cast<DrawObject*>(o->ancestor()) ) &&
                     e->region().contains( o->boundingRect().toRect() ) ) 
                        o->draw( &p );


        p.setBrush( Qt::NoBrush );
        p.setPen( QPen() );

#ifndef QT_NO_DEBUG_OUTPUT
        int elements = stopWatch.elapsed() - papgrid;
#endif
        
        if ( tentativeDraw_ ) 
                foreach ( DrawObject* o, l ) 
                        o->drawTentative( &p );
        else
                foreach ( DrawObject* o, l )
                        o->draw( &p );

        p.resetMatrix();

#ifndef QT_NO_DEBUG_OUTPUT
        int selection = stopWatch.elapsed() - elements - papgrid;
#endif

        controler_->drawActionMetaData( &p, this );
        
        if ( snapped_ ) {
                p.setPen( Qt::green );
                p.drawEllipse( snapRect() );
        }
        
        p.end();

#ifndef QT_NO_DEBUG_OUTPUT
        int total = stopWatch.elapsed();
        int meta = total - selection - elements;
        
        qDebug() << "papgrid: " << papgrid << ", elements: " << elements
                 << ", selection: " << selection << ", meta: " << meta
                 << ", total: " << total;
#endif
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
        return Geom::centerRect( snapPoint_, QSize( 9,9 ) );
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


void CanvasView::updateFigureMetaData()
{
        unit_ = figure_->unit();
        paperSize_ = figure_->paper().size();
        calcGrid();
        vRuler_->setUnit( figure_->unitKey() );
        hRuler_->setUnit( figure_->unitKey() );
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
        QSize s = ( figure_->paper().size() * scale_ ).toSize();
        qDebug() << "doResizing()" << s << figure_->paper().size() << scale_;
        if ( hRuler_ )
                hRuler_->setLength( s.width() );
        if ( vRuler_ )
                vRuler_->setLength( s.height() );
        resize( s );    
}

double CanvasView::paperWidth() const
{
        return figure_->paper().size().width() * scale_;
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
        if ( ! ( snapScaled_ && controler_->actionWantsSnap( p, scaleMatrixInv_ ) ) ) {
                if ( snapped_ )
                        update();
                snapped_ = false;
                return true;
        }

        p.setX( qRound( qRound(qreal(p.x())/snapScaled_) * snapScaled_ ) );
        p.setY( qRound( qRound(qreal(p.y())/snapScaled_) * snapScaled_ ) );

        bool snapped = ( p != snapPoint_ );

        if ( snapped ) {
                oldSnapPoint_ = snapPoint_;
                snapPoint_ = p;
                snapped_ = true;
                update();
        }

        return snapped;
}


void CanvasView::setupStatus()
{
        status_.setInformation( Qt::NoModifier,
                                ActionStatus::Information()
                                .setLeft( tr("choose object") )
                                .setRight( tr("context menu") ) );
}

void CanvasView::takeOverStatusIndicator( bool takeIt )
{
        if ( takeIt )
                emit status( status_ );
}

