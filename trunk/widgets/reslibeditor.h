 
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

#ifndef reslibeditor_h
#define reslibeditor_h

#include "editdialog.h"
#include "resourcekey.h"
#include "typedefs.h"
#include "resourcemodel.h"
#include "resourcedialog.h"

#include <QModelIndex>
#include <QItemSelectionModel>
#include <QMessageBox>
#include <QAction>

class QAction;
class StrokeDemo;

class AbstractReslibEditor : public EditDialog
{
        Q_OBJECT
public:
        AbstractReslibEditor( const ResourceKey& initial, QWidget* parent=0 )
                : EditDialog( parent ),
                  oldResourceKey_( initial )
        {}
        
        ~AbstractReslibEditor() {}

        ResourceKey resourceKey() { return resourceKey_; }
        
protected:
        void init();
        virtual void reset();

private slots:
        virtual void resourceChanged( const QModelIndex& index, const QModelIndex& prev ) = 0;
        virtual void selectionChanged( const QModelIndex& index ) = 0;
        virtual void copyResource() = 0;
        virtual void deleteResource() = 0;
        virtual void editResource() = 0;

protected:
        virtual void setResource( const ResourceKey& key ) = 0;
        
        QAction* copyResourceAction_;
        QAction* deleteResourceAction_;
        QAction* editResourceAction_;

        QAbstractListModel* resourceModel_;
        QItemSelectionModel* selectedResource_;

//         ResourceDemo* resourceDemo_;
        
        ResourceKey oldResourceKey_, resourceKey_;
};

template<typename Resource> class ReslibEditor : public AbstractReslibEditor
{
public:
        ReslibEditor( const ResourceKey& initial, QWidget* parent = 0 )
                : AbstractReslibEditor( initial, parent ),
                  resourceLib( ResLib<Resource>::instance() ),
                  keys_( resourceLib.keys() )
        {
                setupModel();
                init();
        }
        
        ~ReslibEditor() {}

        static ResourceKey getResource( const ResourceKey& initial, bool* ok = 0,
                                        QWidget* parent = 0 );

private:
        virtual void resourceChanged( const QModelIndex& index, const QModelIndex& prev );
        virtual void selectionChanged( const QModelIndex& index );
        virtual void copyResource();
        virtual void deleteResource();
        virtual void editResource();

        virtual void setResource( const ResourceKey& );

        ResLib<Resource>& resourceLib;
        const ResourceKeyList& keys_;

        void setupModel();
};


template<typename Resource>
void ReslibEditor<Resource>::setupModel() 
{
        resourceModel_ = new ResourceModel<Resource>( this );
        selectedResource_ = new QItemSelectionModel( resourceModel_ );

        if ( keys_.contains( resourceKey_ ) )
                selectedResource_->select( resourceModel_->index( keys_.indexOf( resourceKey_ ) ),
                                           QItemSelectionModel::ClearAndSelect );
}

template<typename Resource>        
void ReslibEditor<Resource>::resourceChanged( const QModelIndex& index, const QModelIndex& )
{
        setResource( keys_.at( index.row() ) );
}


template<typename Resource>        
void ReslibEditor<Resource>::copyResource()
{
        QString keyString = resourceKey_.keyString();
        keyString.prepend( tr("copy of ") );

        ResourceKey newKey( keyString, ResourceKey::InFig );

        if ( resourceLib.contains( newKey ) ) {
                QMessageBox::warning( this, tr("Resource not copied!"),
                                      tr("A resource with the name %1 already exists.\n"
                                         "Rename it first and copy again.").arg( keyString ) );
                return;
        }

        resourceLib.insert( newKey, resourceLib[resourceKey_] );
        setResource( resourceKey_ );
        int ind = keys_.indexOf( newKey );
        selectedResource_->select( resourceModel_->index( ind ), QItemSelectionModel::ClearAndSelect );
        resourceModel_->insertRow( ind );
}


template<typename Resource>        
void ReslibEditor<Resource>::deleteResource()
{ 
        int ind = keys_.indexOf( resourceKey_ );
        
        resourceModel_->removeRow( ind );
        
        resourceLib.remove( resourceKey_ );
        
        qDebug() << "Jere";
        if ( ind > keys_.count() )
                ind--;
        
        selectedResource_->select( resourceModel_->index( ind ), QItemSelectionModel::ClearAndSelect );
}

template<typename Resource>        
void ReslibEditor<Resource>::editResource()
{
        Resource resource = resourceLib[resourceKey_];

        if ( ResourceDialog<Resource>::execute( resource ) == QDialog::Accepted  ) {
                resourceLib.remove( resourceKey_ );
                resourceLib.insert( resourceKey_, resource );
        }
}

template<typename Resource>        
void ReslibEditor<Resource>::selectionChanged( const QModelIndex& index )
{
    
}

template<typename Resource>        
void ReslibEditor<Resource>::setResource( const ResourceKey& key )
{
        resourceKey_ = key;

        bool dontTouch = !key.isValid() || key.isBuiltIn();

        deleteResourceAction_->setEnabled( !dontTouch );
        editResourceAction_->setEnabled( !dontTouch );
        selectedResource_->select( resourceModel_->index( keys_.indexOf( resourceKey_ ) ),
                                 QItemSelectionModel::ClearAndSelect );

//        resourceDemo_->setResource( resourceLib[key] );
}

template<typename Resource>        
ResourceKey ReslibEditor<Resource>::getResource( const ResourceKey& initial, bool* ok,
                                                 QWidget* parent )
{
        ReslibEditor<Resource> dlg( initial, parent );

        bool accepted = ( dlg.exec() == QDialog::Accepted );
        if ( ok )
                *ok = accepted;

        ResourceKey result;
        if ( accepted )
                result = dlg.resourceKey();

        return result;
}

#endif
