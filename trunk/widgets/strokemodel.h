 
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

#ifndef strokemodel_h
#define strokemodel_h

#include "stroke.h"
#include "reslib.h"

#include <QAbstractListModel>
#include <QList>


class StrokeModel : public QAbstractListModel
{
        Q_OBJECT
public:
        StrokeModel( const ResourceKeyList& rkeys, QObject* parent = 0 );        

        int rowCount( const QModelIndex& parent = QModelIndex() ) const;
        QVariant data( const QModelIndex& index, int role ) const;
//        QVariant headerData( int, Qt::Orientation, int = Qt::DisplayRole ) const;

        bool setData( const QModelIndex& index, const QVariant& value, int role );
        
        bool insertRows( int position, int rows, const QModelIndex& index = QModelIndex() );
        bool removeRows( int position, int rows, const QModelIndex& index = QModelIndex() );
        
        Qt::ItemFlags flags( const QModelIndex& index ) const;

//        QModelIndex indexOf( const ResourceKey& key );
        
private:
        StrokeLib& strokeLib_;
        const ResourceKeyList& keyList_;
};

inline
int StrokeModel::rowCount( const QModelIndex& ) const
{
        return keyList_.count() ;
}


#endif
