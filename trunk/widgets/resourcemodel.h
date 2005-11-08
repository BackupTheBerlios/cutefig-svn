 
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

#ifndef resourcemodel_h
#define resourcemodel_h

#include "reslib.h"
#include "resourceiconengine.h"

#include <QAbstractListModel>
#include <QList>
#include <QVariant>
#include <QString>
#include <QIcon>

#include <QDebug>

template<typename Resource> class ResourceModel : public QAbstractListModel
{
public:
        ResourceModel<Resource>( QObject* parent = 0 );        

        int rowCount( const QModelIndex& parent = QModelIndex() ) const;
        QVariant data( const QModelIndex& index, int role ) const;

        bool setData( const QModelIndex& index, const QVariant& value, int role );
        
        bool insertRows( int position, int rows, const QModelIndex& index = QModelIndex() );
        bool removeRows( int position, int rows, const QModelIndex& index = QModelIndex() );
        
        Qt::ItemFlags flags( const QModelIndex& index ) const;

        
private:
        ResLib<Resource>& resourceLib_;
};

template<typename Resource>
ResourceModel<Resource>::ResourceModel( QObject* parent )
        : QAbstractListModel( parent ),
          resourceLib_( ResLib<Resource>::instance() )
{
}

template<typename Resource>
QVariant ResourceModel<Resource>::data( const QModelIndex& index, int role ) const
{
        QVariant data;

        int i = index.row();
        const ResourceKey k = resourceLib_.at(i);
        
        switch ( role ) {
            case Qt::DisplayRole:
            case Qt::EditRole:
                    data.setValue( k.keyString() ); break;
            case Qt::DecorationRole:
                    data.setValue( QIcon( new ResourceIconEngine<Resource>( resourceLib_[k] ) ) );
                    break;
            default: break;
        }

        return data;
}

template<typename Resource>
Qt::ItemFlags ResourceModel<Resource>::flags( const QModelIndex& index ) const
{
        Qt::ItemFlags flags = QAbstractListModel::flags( index );

        if ( !index.isValid() )
                return flags;
        
        flags |= Qt::ItemIsUserCheckable;

        if ( !resourceLib_.at( index.row() ).isBuiltIn() )
                flags |= Qt::ItemIsEditable;
        return flags;
}

// we don't need to change keyList_ as Reslib::insert() and
// ResLib::remove() do it.
template<typename Resource>
bool ResourceModel<Resource>::setData( const QModelIndex& index, const QVariant& value, int )
{
        ResourceKey newKey( value.toString(), ResourceKey::InLib );

        if ( resourceLib_.contains( newKey ) )
                return false;

        int i = index.row();
        
        ResourceKey oldKey = resourceLib_.at( i );
        Resource resource = resourceLib_[oldKey];

        return true;
}

template<typename Resource>
bool ResourceModel<Resource>::insertRows( int position, int rows, const QModelIndex& )
{
        beginInsertRows( QModelIndex(), position, position+rows-1 );
        endInsertRows();
        return true;
}

template<typename Resource>
bool ResourceModel<Resource>::removeRows( int position, int rows, const QModelIndex& parent )
{
        beginRemoveRows( parent, position, position+rows-1 );
        endRemoveRows();
        return true;
}


template<typename Resource>
inline int ResourceModel<Resource>::rowCount( const QModelIndex& ) const
{
        return resourceLib_.count() ;
}


#endif
