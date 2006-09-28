 
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

#include "centralwidget.h"
#include "canvasview.h"
#include "ruler.h"

#include <QScrollArea>
#include <QScrollBar>
#include <QLayout>
#include <QTimer>

/*! \class CentralWidget
 *
 *  It basically hosts the two Rulers and the CanvasView. Maybe a
 *  QSplitter will be added together with some sort of structure view.
 */

/*! Constructs all the child widgets, connects signals and slots and
 * then resizes the rulers by calling resize rulers by a QTimer.
 */
CentralWidget::CentralWidget( CanvasView* cv, QMainWindow * parent )
        : QWidget( parent )
{
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        scrollArea_ = new QScrollArea( this );
        scrollArea_->setWidget( cv );
	scrollArea_->setFrameStyle( QFrame::Panel | QFrame::Sunken );

	cv->setHorizontalScrollBar( scrollArea_->horizontalScrollBar() );
	cv->setVerticalScrollBar( scrollArea_->verticalScrollBar() );
	
	RulerDispatcher* rd = new RulerDispatcher( cv->size(), this );
	
        connect( scrollArea_->horizontalScrollBar(), SIGNAL( valueChanged(int) ),
		 rd, SLOT( horizontalScroll(int) ) );
        connect( scrollArea_->verticalScrollBar(), SIGNAL( valueChanged(int) ),
                 rd, SLOT( verticalScroll(int) ) );

        connect( cv, SIGNAL( cursorMovedTo(const QPoint&) ), rd, SLOT( setPos(const QPoint&) ) );
        connect( cv, SIGNAL( sizeChanged(const QSize&) ), rd, SLOT( sizeChange(const QSize&) ) );

        connect( cv, SIGNAL( cursorIsIn(bool) ), rd, SLOT( setIndicating(bool) ) );

	connect( cv, SIGNAL( matrixChanged(const QMatrix&) ),
		 rd, SLOT( setMatrix(const QMatrix&) ) );

        connect( cv, SIGNAL(unitChanged(const ResourceKey&)),
                 rd, SLOT(setUnit(const ResourceKey&)) );

        QGridLayout* l = new QGridLayout( this );
        l->setMargin( 0 );
        l->addWidget( rd->unitLabel() ); 
        l->addWidget( rd->horizontalRuler(), 0,1 );
        l->addWidget( rd->verticalRuler(), 1,0 );
        l->addWidget( scrollArea_, 1,1 );

}


QSize CentralWidget::viewportSize() const
{
	return scrollArea_->viewport()->size();
}
