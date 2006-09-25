 
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

/*! \class CanvasView 
 *  
 *  \brief This is the widget used to display the figure. It also
 *  handles mouse and keyboard events.
*/

#ifndef canvasview_h
#define canvasview_h

#include "viewbase.h"
#include "controler.h"
#include "actionstatus.h"

#include <QPixmap>
//#include <QGLWidget>
#include <QScrollArea>
#include <QPaintEvent>
#include <QPainter>
#include <QList>

class DrawObject;
class CuteFig;
class ActionStatusIndicator;

class QPainter;
class QPointF;
class QRectF;

typedef QList<InteractiveAction*> ActionList;

//! Main user interaction widget
/*! This class is responsible to display the figure to the user and to
 *  handle user interaction. It accepts key and mouse events, figures
 *  out whether they are to be handled and tells the Controler to call
 *  InteractiveAction::click(), InteractiveAction::move() or
 *  InteractiveAction::keyStroke() respectively.
 *
 *  The second job of CanvasView is to pass a QPainter to the Figure
 *  to which the DrawObjects can be painted to. Furthermore it draws
 *  the paper and the snap grid. Drawing the helplines is also planned.
 */
class CanvasView : public QWidget, public ViewBase
{
        Q_OBJECT
public:
        //! The constructor needs a Controler \param c and a Figure \param f.
        CanvasView( Controler* c, ActionStatusIndicator* si, const Figure* f, CuteFig* parent =0 );

        ~CanvasView() { }

        //! Redraws the whole figure, o is the DrawObject just edited.
        void updateFigure( bool tentative = false );
        void drawSelection( QPainter* p ) const;
        
        //< Draws the control points of all considerable objects
        void drawObjectsPoints( QPainter* p, const DrawObject* o ) const; 

        void drawObjectsMetaData( QPainter* p, const DrawObject* o ) const;

        const Controler* controler() const { return controler_; }
        CuteFig* mainWindow() { return mainWindow_; }

        double paperWidth() const;        

        
public slots:
        void updateFigureMetaData();

        void zoomIn();   //!< Zooms in by 10% of the original size
        void zoomOut();  //!< Zooms out by 10% of the original size
        void zoomOrig();
        void zoomFit();
        void setZoom( double z ); //!< sets the zoom to \param z * 100%.

        void newSnapGrid();
        void refineGrid();
        void corsenGrid();

        void setGridWidth( double gridWidth  ); //!< sets the width of the grid
        void setSnapWidth( double snapWidth );  //!< sets the width of the snapgrid

signals:
        void status( const ActionStatus& ); 
        //!< emits the status of the view for a window's status bar 

        void cursorMovedTo( const QPoint& );
        void cursorIsIn( bool );
        
        //! To be emitted when the zoom scale changes.
	void zoomChanged( double );
	void sizeChanged( const QSize& );
	void matrixChanged( const QMatrix& );
        void unitChanged( const ResourceKey& );
     
protected:
        virtual void mouseReleaseEvent( QMouseEvent * e );
//        virtual void mousePressEvent( QMouseEvent* e );
        virtual void mouseDoubleClickEvent( QMouseEvent * e );
        virtual void mouseMoveEvent( QMouseEvent * e );
        virtual void contextMenuEvent( QContextMenuEvent* e );

        virtual void keyPressEvent( QKeyEvent* e );
        virtual void keyReleaseEvent( QKeyEvent* e );
        
        virtual bool event( QEvent* e );
        virtual void inputMethodEvent( QInputMethodEvent* e );
        virtual QVariant inputMethodQuery( Qt::InputMethodQuery q );

        virtual void paintEvent( QPaintEvent* e );

        virtual void enterEvent( QEvent* e );
        virtual void leaveEvent( QEvent* e );

        virtual bool eventFilter( QObject* watched, QEvent* event );

private slots:
        void takeOverStatusIndicator( bool takeIt );
        
private:
        void drawPaper( QPainter* p ); //!< draws the paper
        void drawGrid( QPainter* p );  //!< draws the snap grid
        QRect snapRect(); //!< emphesizes the point snaped to

        void setZoom_private( double z );

        void doResizing();

        void calcGrid(); //!< calculates the snapgrid according to scale_

        bool snap( QPoint& p ); 
        //!< snaps p to the grid if the oldsnappoint_ is different from p

        void handleReturnHit();

        QRegion drawingRegion() const;

        void setupStatus();

        void dispatchModifierChange( const QKeyEvent* e );

        CuteFig* mainWindow_;

        QMatrix scaleMatrix_, scaleMatrixInv_; //!< the scaling matrix
        double scale_, zoom_;   
        double gridWidth_;
        double gridScaled_;
        QPointF gridOffset_;

        double snapWidth_;
        double snapScaled_;
        QPointF snapOffset_;

        double unit_;
        QSizeF paperSize_;

        QPoint offset_;
        QRegion oldRegion_;

        QPoint snapPoint_, oldSnapPoint_;
        bool snapped_;

        bool tentativeDraw_;

        ActionStatus status_;

        ActionStatusIndicator* statusIndicator_;
};

#endif
