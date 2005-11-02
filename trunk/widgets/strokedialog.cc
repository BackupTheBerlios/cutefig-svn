 
/*****************************************************************************
**
**  $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2004 Johannes Mueller, joh@users.berlios.de
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

#include "strokedialog.h"
#include "strokelib.h"
#include "strokemodel.h"
#include "actionpushbutton.h"
#include "strokedemo.h"
#include "colordialog.h"
#include "resourcedialog.h"

#include <QAction>
#include <QLayout>
#include <QItemSelectionModel>
#include <QListView>
#include <QInputDialog>
#include <QMessageBox>

#include <QDebug>

StrokeDialog::StrokeDialog( const ResourceKey& strokeKey, const ResourceKeyList& keys,
                            QWidget* parent )
        : EditDialog( parent ),
          oldStrokeKey_( strokeKey ),
          strokeLib( StrokeLib::instance() ),
          keys_ ( keys.isEmpty() ? strokeLib.keys() : keys )
{
        strokeModel_ = new StrokeModel( keys_ );
        selectedStroke_ = new QItemSelectionModel( strokeModel_ );
        connect( selectedStroke_, SIGNAL( currentChanged(const QModelIndex&, const QModelIndex&) ),
                 this, SLOT( strokeChanged( const QModelIndex&, const QModelIndex& ) ) );

        if ( keys_.contains( strokeKey ) )
                selectedStroke_->select( strokeModel_->index( keys_.indexOf( strokeKey ) ),
                                         QItemSelectionModel::ClearAndSelect );
        
        copyStrokeAction_ = new QAction( tr("&Copy"), this );
//        copyStrokeAction_->setShortcutContext( Qt::ApplicationShortcut );
        copyStrokeAction_->setShortcut( Qt::CTRL+Qt::Key_C );
        connect( copyStrokeAction_, SIGNAL( triggered() ), this, SLOT( copyStroke() ) );
        
        deleteStrokeAction_ = new QAction( tr("&Delete"), this );
        deleteStrokeAction_->setShortcut( Qt::Key_Delete );
        connect( deleteStrokeAction_, SIGNAL( triggered() ), this, SLOT( deleteStroke() ) );

        editStrokeAction_ = new QAction( tr("&Edit"), this );
        editStrokeAction_->setShortcut( Qt::CTRL+Qt::Key_E );
        connect( editStrokeAction_, SIGNAL( triggered() ), this, SLOT( editStroke() ) );

        QHBoxLayout* mainLayout = new QHBoxLayout();
        QListView* strokeListView = new QListView( this );
        strokeListView->setModel( strokeModel_ );
        strokeListView->setSelectionModel( selectedStroke_ );
        mainLayout->addWidget( strokeListView );


        QVBoxLayout* editButtonLayout = new QVBoxLayout();

        ActionPushButton* copyStrokeButton = new ActionPushButton( copyStrokeAction_, this );
        editButtonLayout->addWidget( copyStrokeButton );

        ActionPushButton* editStrokeButton = new ActionPushButton( editStrokeAction_, this );
        editButtonLayout->addWidget( editStrokeButton );

        ActionPushButton* deleteStrokeButton = new ActionPushButton( deleteStrokeAction_, this );
        editButtonLayout->addWidget( deleteStrokeButton );


        qDebug() << editStrokeAction_->text() << editStrokeButton->text();

        mainLayout->addLayout( editButtonLayout );


        strokeDemo_ = new StrokeDemo( strokeLib[strokeKey], this );
        mainLayout->addWidget( strokeDemo_ );
        
        dialogLayout_->insertLayout( 0, mainLayout );

        setStroke( strokeKey );
        
        reset();
}

void StrokeDialog::reset()
{
        setStroke( oldStrokeKey_ );
}

void StrokeDialog::strokeChanged( const QModelIndex& index, const QModelIndex& )
{
        setStroke( keys_.at( index.row() ) );
}

void StrokeDialog::copyStroke()
{
        QString keyString = strokeKey_.keyString();
        keyString.prepend( tr("copy of ") );

        ResourceKey newKey( keyString, ResourceKey::InFig );

        if ( strokeLib.contains( newKey ) ) {
                QMessageBox::warning( this, tr("Stroke not copied!"),
                                      tr("A stroke with the name %1 already exists.\n"
                                         "Rename it first and copy again.").arg( keyString ) );
                return;
        }

        strokeLib.insert( newKey, strokeLib[strokeKey_] );
        setStroke( strokeKey_ );
        int ind = keys_.indexOf( newKey );
        selectedStroke_->select( strokeModel_->index( ind ), QItemSelectionModel::ClearAndSelect );
        strokeModel_->insertRow( ind );
}


void StrokeDialog::deleteStroke()
{ 
        int ind = keys_.indexOf( strokeKey_ );
        
        strokeModel_->removeRow( ind );
        
        strokeLib.remove( strokeKey_ );
        
        qDebug() << "Jere";
        if ( ind > keys_.count() )
                ind--;
        
        selectedStroke_->select( strokeModel_->index( ind ), QItemSelectionModel::ClearAndSelect );
}

void StrokeDialog::editStroke()
{
        Stroke stroke = strokeLib[strokeKey_];

        bool ok = false;
        
        switch( stroke.type() ) {
            case Stroke::sColor: {
                    QColor c = ColorDialog::getColor( stroke.color(), &ok );
                    if ( ok )
                            stroke.setColor( c );
                    break;
            }
            case Stroke::sGradient: 
            {
                    Gradient g = ResourceDialog<Gradient>::editData( stroke.gradient(), &ok );
                    if ( ok ) 
                            stroke.setGradient( g );
                    break;
            }
            default: break;
        }

        if ( ok ) {
                strokeLib.remove( strokeKey_ );
                strokeLib.insert( strokeKey_, stroke );
        }
}

void StrokeDialog::selectionChanged( const QModelIndex& index )
{
    
}

void StrokeDialog::setStroke( const ResourceKey& key )
{
        strokeKey_ = key;

        bool dontTouch = !key.isValid() || key.isBuiltIn();

        deleteStrokeAction_->setEnabled( !dontTouch );
        editStrokeAction_->setEnabled( !dontTouch );
        selectedStroke_->select( strokeModel_->index( keys_.indexOf( strokeKey_ ) ),
                                 QItemSelectionModel::ClearAndSelect );

        strokeDemo_->setStroke( strokeLib[key] );
}

ResourceKey StrokeDialog::getStroke( const ResourceKey& initial, bool* ok,
                                     const ResourceKeyList& keys,
                                     QWidget* parent )
{
        StrokeDialog dlg( initial, keys, parent );

        bool accepted = ( dlg.exec() == QDialog::Accepted );
        if ( ok )
                *ok = accepted;

        ResourceKey result;
        if ( accepted )
                result = dlg.strokeKey();

        return result;
}
