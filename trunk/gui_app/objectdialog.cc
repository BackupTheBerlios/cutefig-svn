 
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
#include "progutils.h"
#include "lineshowwidget.h"
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

        tabWidget = new QTabWidget( this );
        dialogLayout()->insertWidget( 0, tabWidget );

        setUpGeneral();
        
        QTimer::singleShot( 0, this, SLOT( init() ) );
} 

void ObjectDialog::setUpGeneral()
{
        qDebug("ObjectDialog::setUpGeneral");

        QWidget* page = new QWidget();
        tabWidget->addTab( page, "&General" );

        QGridLayout* layout = new QGridLayout( page );
    
        // final setup

        penWidget = new PenWidget( tr("&Pen"), page );
        layout->addWidget( penWidget, 0,0 );

        lineStroke = new StrokeWidget( tr("&Line stroke"), page );
        layout->addWidget( lineStroke, 0,1 );

        fillStroke = new StrokeWidget( tr("&Fill"), page );
        layout->addWidget( fillStroke, 1,0 );

        arrows = new ArrowWidget( tr("&Arrows"), page );
        layout->addWidget( arrows, 1,1 );
        arrows->setEnabled( drawObject_->canHaveArrows() );
        
        depth = new QSpinBox( page );
        depth->setMinimum(0);
        depth->setMaximum(999);
        depth->setSingleStep(1);

        Layouter( new QHBoxLayout() )
                .labeledWidget( tr("&Depth"), depth )
                .finishTo( layout, 2,0 );
        
        
        
        // Comment

        comment = new QTextEdit( page );
//        comment->setTextFormat( Qt::PlainText );
        comment->setTabChangesFocus( true );
        Layouter( new QVBoxLayout() )
                .labeledWidget( tr("&Comment"), comment )
                .finishTo( layout,2,1 );        
}


// void ObjectDialog::accept()
// {
//         if ( comment->document()->isModified() ) {
//                 drawObject_->setComment( comment->toPlainText() );
//                 action_->wObjectHasChanged();
//         }

//         QDialog::accept();
// }

void ObjectDialog::doReset()
{
        DrawObject* o = action_->restoreWObject();
        if ( !o ) {
                qDebug("WARNING !!! restoring wObject failed...");
                return;
        }

        setDrawObject( o );

        setDefaultValues();
        setUpConnections();
}


void ObjectDialog::setDefaultValues()
{ 
        qDebug("ObjectDialog::setDefaultValues");
        
//        lineShow->disconnect();
//        fillPattern->disconnect();
        depth->disconnect();

        penWidget->setPen( drawObject_->pen() );
        lineStroke->setStroke( drawObject_->stroke() );
        fillStroke->setStroke( drawObject_->fill() );

        arrows->setArrows( drawObject_->startArrow(), drawObject_->endArrow() );
        
        depth->setValue( drawObject_->depth() );
        comment->setPlainText( drawObject_->comment() );

        setDefaultValuesPrivate();
}


void ObjectDialog::commitChanges( QObject* )
{        
        drawObject_->setPen( penWidget->pen() );
        drawObject_->setStroke( lineStroke->stroke() );
        drawObject_->setFill( fillStroke->stroke() );
        drawObject_->setDepth( depth->value() );
        drawObject_->setStartArrow( arrows->startArrow() );
        drawObject_->setEndArrow( arrows->endArrow() );
}


void ObjectDialog::setUpConnections()
{
        qDebug("ObjectDialog::setUpConnections");

        connect( penWidget, SIGNAL( penChanged() ), this, SLOT( noticeChange() ) );
        connect( lineStroke, SIGNAL( strokeChanged() ), this, SLOT( noticeChange() ) );
        connect( fillStroke, SIGNAL( strokeChanged() ), this, SLOT( noticeChange() ) );
        
//        connect( depth, SIGNAL( valueChanged(int) ), drawObject_, SLOT( setDepth(int) ) );        
        connect( depth, SIGNAL( valueChanged( int ) ), this, SLOT( noticeChange() ) );

        connect( arrows, SIGNAL( startArrowChanged(const Arrow&) ), this, SLOT( noticeChange() ) );
        connect( arrows, SIGNAL( endArrowChanged(const Arrow&) ), this, SLOT( noticeChange() ) );
}

void ObjectDialog::init()
{
        castDrawObject();
        
        setDefaultValues();
        setUpConnections();
}

void ObjectDialog::setDrawObject( DrawObject* o )
{
        drawObject_ = o;
        castDrawObject();
}

// void ObjectDialog::changeStartArrow( const Arrow& a )
// {
//         drawObject_->setStartArrow( a );
//         this->noticeChange();
// }

// void ObjectDialog::changeEndArrow( const Arrow& a )
// {
//         drawObject_->setEndArrow( a );
//         this->noticeChange();
// }
