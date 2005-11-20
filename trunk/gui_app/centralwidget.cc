 
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

/** \class CentralWidget
 *
 *  It basically hosts the two Rulers and the CanvasView. Maybe a
 *  QSplitter will be added together with some sort of structure view.
 */

/** Constructs all the child widgets, connects signals and slots and
 * then resizes the rulers by calling resize rulers by a QTimer.
 */
CentralWidget::CentralWidget( CanvasView* cv, QMainWindow * parent )
        : QWidget( parent )
{
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QScrollArea* sa = new QScrollArea( this );
        sa->setWidget( cv );
        viewport_ = sa->viewport();

        hRuler_ = new Ruler( viewport_->width(), Qt::Horizontal, this );
        vRuler_ = new Ruler( viewport_->height(), Qt::Vertical, this );

        cv->setHRuler( hRuler_ );
        cv->setVRuler( vRuler_ );

        connect( cv, SIGNAL( scaleChanged( double ) ), hRuler_, SLOT( setScale( double ) ) );
        connect( cv, SIGNAL( scaleChanged( double ) ), vRuler_, SLOT( setScale( double ) ) );
        connect( sa->horizontalScrollBar(), SIGNAL( valueChanged(int) ), 
                 hRuler_, SLOT( setStart( int ) ) );
        connect( sa->verticalScrollBar(), SIGNAL( valueChanged(int) ),
                 vRuler_, SLOT( setStart( int ) ) );    

        QGridLayout* l = new QGridLayout( this );
        QWidget* corner = new QWidget( this );
        corner->setFixedSize( 30, 30 );
        l->addWidget( corner );
        l->addWidget( hRuler_, 0,1 );
        l->addWidget( vRuler_, 1,0 );
        l->addWidget( sa, 1,1 );

        QTimer::singleShot( 0, this, SLOT( resizeRulers() ) );
}

void CentralWidget::resizeRulers()
{
        hRuler_->setLength( viewport_->width() );
        vRuler_->setLength( viewport_->height() );    
}
