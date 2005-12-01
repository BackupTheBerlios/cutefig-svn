 
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
#include "penwidget.h"
#include "strokewidget.h"
#include "editdialogaction.h"


ObjectDialog::ObjectDialog( DrawObject* o, EditdialogAction* a, QWidget* parent )
        : EditDialog( parent ),
          action_ ( a )
{ 
        qDebug("ObjectDialog::ObjectDialog");
        drawObject_ = o;

        tabWidget = new QTabWidget( this );
        dialogLayout_->insertWidget( 0, tabWidget );

        setUpGeneral();
        
        QTimer::singleShot( 0, this, SLOT( init() ) );
} 

void ObjectDialog::setUpGeneral()
{
        qDebug("ObjectDialog::setUpGeneral");

        QWidget* page = new QWidget();
        tabWidget->addTab( page, "&General" );

        QGridLayout* layout = new QGridLayout( page );
//        ProgUtils::prepareLayout( topLayout, false );
    
        // final setup

        penWidget = new PenWidget( tr("&Pen"), page );
        layout->addWidget( penWidget, 0,0 );

        lineStroke = new StrokeWidget( tr("&Line stroke"), page );
        layout->addWidget( lineStroke, 0,1 );

        fillStroke = new StrokeWidget( tr("&Fill"), page );
        layout->addWidget( fillStroke, 1,0 );
        
        QHBoxLayout* dlt = new QHBoxLayout();
        layout->addItem( dlt, 2,0 );
        depth = new QSpinBox( page );
        depth->setMinimum(0);
        depth->setMaximum(999);
        depth->setSingleStep(1);
        QLabel* dlb = new QLabel( tr("&Depth"), page );
        dlb->setBuddy( depth );
        dlt->addWidget( dlb );
        dlt->addWidget( depth );

        // Comment

        QVBoxLayout* clt = new QVBoxLayout();
        layout->addItem( clt, 2,1 );
        comment = new QTextEdit( page );
//        comment->setTextFormat( Qt::PlainText );
        comment->setTabChangesFocus( true );
        QLabel* clb = new QLabel( tr("&Comment"), page );
        clb->setBuddy( comment );
        clt->addWidget( clb );
        clt->addWidget( comment );
}


void ObjectDialog::accept()
{
        if ( comment->document()->isModified() ) {
                drawObject_->setComment( comment->toPlainText() );
                action_->wObjectHasChanged();
        }

        QDialog::accept();
}

void ObjectDialog::reset()
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

        penWidget->setPen( drawObject_->p_pen() );
        lineStroke->setStroke( drawObject_->p_stroke() );
        fillStroke->setStroke( drawObject_->p_fill() );

        depth->setValue( drawObject_->depth() );
        comment->setPlainText( drawObject_->comment() );

        setDefaultValuesPrivate();
}



void ObjectDialog::setUpConnections()
{
        qDebug("ObjectDialog::setUpConnections");

        connect( penWidget, SIGNAL( penChanged() ), action_, SLOT( wObjectHasChanged() ) );
        connect( lineStroke, SIGNAL( strokeChanged() ), action_, SLOT( wObjectHasChanged() ) );
        connect( fillStroke, SIGNAL( strokeChanged() ), action_, SLOT( wObjectHasChanged() ) );
        
        connect( depth, SIGNAL( valueChanged(int) ), drawObject_, SLOT( setDepth(int) ) );
        
        connect( depth, SIGNAL( valueChanged( int ) ), action_, SLOT( wObjectHasChanged() ) );

        setUpConnectionsPrivate();
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
