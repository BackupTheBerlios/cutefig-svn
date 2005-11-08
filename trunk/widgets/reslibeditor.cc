 
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

#include "reslibeditor.h"
#include "actionpushbutton.h"
//#include "resourcedemo.h"
#include "colordialog.h"
#include "resourcedialog.h"

#include <QAction>
#include <QLayout>
#include <QItemSelectionModel>
#include <QListView>
#include <QInputDialog>
#include <QMessageBox>

#include <QDebug>


void AbstractReslibEditor::init()
{
        connect( selectedResource_, SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
                 this, SLOT( resourceChanged( const QModelIndex&, const QModelIndex& ) ) );

        connect( resourceModel_, SIGNAL( dataChanged( const QModelIndex&, const QModelIndex& ) ),
                 this, SLOT( resourceChanged( const QModelIndex&, const QModelIndex& ) ) );
        
        copyResourceAction_ = new QAction( tr("&Copy"), this );
//        copyResourceAction_->setShortcutContext( Qt::ApplicationShortcut );
        copyResourceAction_->setShortcut( Qt::CTRL+Qt::Key_C );
        connect( copyResourceAction_, SIGNAL( triggered() ), this, SLOT( copyResource() ) );
        
        deleteResourceAction_ = new QAction( tr("&Delete"), this );
        deleteResourceAction_->setShortcut( Qt::Key_Delete );
        connect( deleteResourceAction_, SIGNAL( triggered() ), this, SLOT( deleteResource() ) );

        editResourceAction_ = new QAction( tr("&Edit"), this );
        editResourceAction_->setShortcut( Qt::CTRL+Qt::Key_E );
        connect( editResourceAction_, SIGNAL( triggered() ), this, SLOT( editResource() ) );

        QHBoxLayout* mainLayout = new QHBoxLayout();
        QListView* resourceListView = new QListView( this );
        resourceListView->setModel( resourceModel_ );
        resourceListView->setSelectionModel( selectedResource_ );
        mainLayout->addWidget( resourceListView );


        QVBoxLayout* editButtonLayout = new QVBoxLayout();

        ActionPushButton* copyResourceButton = new ActionPushButton( copyResourceAction_, this );
        editButtonLayout->addWidget( copyResourceButton );

        ActionPushButton* editResourceButton = new ActionPushButton( editResourceAction_, this );
        editButtonLayout->addWidget( editResourceButton );

        ActionPushButton* deleteResourceButton = new ActionPushButton(deleteResourceAction_, this);
        editButtonLayout->addWidget( deleteResourceButton );

        mainLayout->addLayout( editButtonLayout );


//        resourceDemo_ = new ResourceDemo( resourceLib[resourceKey], this );
//        mainLayout->addWidget( resourceDemo_ );
        
        dialogLayout_->insertLayout( 0, mainLayout );

        setResource( resourceKey_ );
        
        reset();
}

void AbstractReslibEditor::reset()
{
        setResource( oldResourceKey_ );
}

