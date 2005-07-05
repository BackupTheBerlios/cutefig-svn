 
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

#ifndef reslib_h
#define reslib_h

#include <QVector>
#include <QHash>

template<typename Key, class Resource> class ResLib
{
public:

        static ResLib<Key, Resource>& instance()
        {
                static ResLib<Key, Resource> inst;
                return inst;
        };

        Resource& operator[]( const Key& key ) { return hash_[key]; }
        const Key key( const Resource& data ) { return hash_[data]; }
        
private:
        ResLib<Key, Resource>() : hash_( QHash<Key, Resource>() ) {};
        ResLib<Key, Resource>( const ResLib<Key, Resource>& ) {}
        QHash<Key, Resource> hash_;
};

template<class Resource> class ResLib<int, Resource> 
{
public:
        static ResLib<int, Resource>& instance()
        {
                static ResLib<int, Resource> inst;
                return inst;
        }

        Resource& operator[]( const int& key ) { return vect_[key]; }
        const int key( const Resource& data ) const { return vect_.indexOf( data ); }
        const int size() const { return vect_.size(); }

        ResLib<int, Resource>& operator<<( const Resource& data ) { vect_ << data; return *this; }
        
private:
        ResLib<int, Resource>() : vect_( QVector<Resource>() ) {}
        ResLib<int, Resource>( const ResLib<int, Resource>& ) {}
        QVector<Resource> vect_;
};


#endif
