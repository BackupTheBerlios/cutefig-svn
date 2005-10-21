 
/*****************************************************************************
**
**  $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2004 Johannes Mueller, johmue@users.sourceforge.net
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

#include "strokemodel.h"
#include "reslib.h"
#include "strokelib.h"
#include "strokeiconengines.h"

#include <QDebug>

StrokeModel::StrokeModel( const ResourceKeyList& rkeys, QObject* parent )
        : QAbstractListModel( parent ),
          strokeLib_( StrokeLib::instance() ),
          keyList_( rkeys )
{
}

QVariant StrokeModel::data( const QModelIndex& index, int role ) const
{
        QVariant data;

        int i = index.row();

        switch ( role ) {
            case Qt::DisplayRole:
            case Qt::EditRole:
                    data.setValue<QString>( keyList_.at(i).keyString() ); break;
            case Qt::DecorationRole: 
                    data.setValue<QIcon>(QIcon(new StrokeIconEngine(strokeLib_[keyList_.at(i)])));
                break;
            default: break;
        }

        return data;
}

Qt::ItemFlags StrokeModel::flags( const QModelIndex& index ) const
{
        Qt::ItemFlags flags = QAbstractListModel::flags( index );

        if ( !index.isValid() )
                return flags;
        
        flags |= Qt::ItemIsUserCheckable;

        if ( !keyList_.at( index.row() ).isBuiltIn() )
                flags |= Qt::ItemIsEditable;
        return flags;
}

// we don't need to change keyList_ as StrokeLib::insert() and
// StrokeLib::remove() do it.
bool StrokeModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
        ResourceKey newKey( value.toString(), ResourceKey::InLib );

        qDebug() << "setData()" << newKey.keyString();
        if ( strokeLib_.contains( newKey ) )
                return false;

        qDebug() << "still here";
        int i = index.row();
        
        ResourceKey oldKey = keyList_.at( i );
        Stroke stroke = strokeLib_[oldKey];
        
        if ( !strokeLib_.insert( newKey, stroke ) )
                qDebug() << "Insert failed" << newKey.keyString();
        if ( !strokeLib_.remove( oldKey ) )
                qDebug() << "Remove failed" << oldKey.keyString();

        return true;
}

bool StrokeModel::insertRows( int position, int rows, const QModelIndex& )
{
        beginInsertRows( QModelIndex(), position, position+rows-1 );
        endInsertRows();
        return true;
}

bool StrokeModel::removeRows( int position, int rows, const QModelIndex& parent )
{
        qDebug() << "removeRows" << position;
        beginRemoveRows( parent, position, position+rows-1 );
        endRemoveRows();
        return true;
}


// QModelIndex StrokeModel::indexOf( const ResourceKey& key )
// {
//         QModelIndex index;
//         if ( keyList_.contains( key ) )
//                 index.
// }
