 
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

#include <QtGui>

#include "cutefig.h"

#include "drawobject.h"
#include "pen.h"
#include "reslib.h"
#include "objectdialog.h"
#include "layouter.h"
#include "penwidget.h"
#include "strokewidget.h"
#include "arrowwidget.h"
#include "editdialogaction.h"


ObjectDialog::ObjectDialog( DrawObject* o, EditdialogAction* a, QWidget* parent )
        : EditDialog( parent ),
          action_ ( a )
{ 
        qDebug("ObjectDialog::ObjectDialog");
        setWindowTitle( tr("%1 properties").arg( o->objectname_stripped() ) );
        drawObject_ = o;

        tabWidget_ = new QTabWidget( this );
        takeWidget( tabWidget_ );

        QWidget* page = new QWidget();
        tabWidget_->addTab( page, "&General" );

        QGridLayout* layout = new QGridLayout( page );
    

	penWidget_ = new PenWidget( tr("&Pen"), page );
        layout->addWidget( penWidget_, 0,0 );
        connect( penWidget_, SIGNAL( penChanged() ), this, SLOT( noticeChange() ) );

        lineStroke_ = new StrokeWidget( tr("&Line stroke"), page );
        layout->addWidget( lineStroke_, 0,1 );
        connect( lineStroke_, SIGNAL( strokeChanged() ), this, SLOT( noticeChange() ) );

        fillStroke_ = new StrokeWidget( tr("&Fill"), page );
        layout->addWidget( fillStroke_, 1,0 );
        connect( fillStroke_, SIGNAL( strokeChanged() ), this, SLOT( noticeChange() ) );

        arrows_ = new ArrowWidget( tr("&Arrows"), page );
        layout->addWidget( arrows_, 1,1 );
        arrows_->setEnabled( drawObject_->canHaveArrows() );
        connect( arrows_, SIGNAL(startArrowChanged(const Arrow&)), this, SLOT(noticeChange()) );
        connect( arrows_, SIGNAL(endArrowChanged(const Arrow&)), this, SLOT(noticeChange()) );
        
        depth_ = new QSpinBox( page );
        depth_->setMinimum(0);
        depth_->setMaximum(999);
        depth_->setSingleStep(1);

        Layouter( new QHBoxLayout() )
                .labeledWidget( tr("&Depth"), depth_ )
                .finishTo( layout, 2,0 );
        
        
        comment_ = new QTextEdit( page );
        comment_->setTabChangesFocus( true );
        Layouter( new QVBoxLayout() )
                .labeledWidget( tr("&Comment"), comment_ )
                .finishTo( layout,2,1 );        
	connect( comment_, SIGNAL( textChanged() ), this, SLOT( noticeChange() ) );        
} 


void ObjectDialog::accept()
{
	drawObject_->updateEverything();
	EditDialog::accept();
}


void ObjectDialog::doReset()
{
        DrawObject* o = action_->restoreWObject();
        Q_ASSERT( o );
        
        drawObject_ = o;
}


void ObjectDialog::setupInitialValues()
{ 
        penWidget_->setPen( drawObject_->pen() );
        lineStroke_->setStroke( drawObject_->stroke() );
        fillStroke_->setStroke( drawObject_->fill() );

        arrows_->setArrows( drawObject_->startArrow(), drawObject_->endArrow() );
        
        depth_->disconnect();
        depth_->setValue( drawObject_->depth() );
        connect( depth_, SIGNAL( valueChanged( int ) ), this, SLOT( noticeChange() ) );

        comment_->setPlainText( drawObject_->comment() );

        castDrawObject();
        setupInitialValuesPrivate();
}


void ObjectDialog::commitChanges()
{        
        drawObject_->setPen( penWidget_->pen() );
        drawObject_->setStroke( lineStroke_->stroke() );
        drawObject_->setFill( fillStroke_->stroke() );
        drawObject_->setDepth( depth_->value() );
        drawObject_->setStartArrow( arrows_->startArrow() );
        drawObject_->setEndArrow( arrows_->endArrow() );
	drawObject_->setComment( comment_->toPlainText() );

	commitChangesPrivate();
	drawObject_->update();
}


