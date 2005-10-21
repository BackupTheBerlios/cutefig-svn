 
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
#include <QMap>

#include "typedefs.h"
#include "resourcekey.h"

class ResLibInit;

template<class Resource> class ResLib
{
public:
        friend class ResLibInit;
        friend class ResLib<Stroke>;
        
        static ResLib<Resource>& instance()
        {
                static ResLib<Resource> inst;
                return inst;
        };

        bool insert( const ResourceKey& key, const Resource& data )
        {
                if ( key.isBuiltIn() )
                        return false;
                
                map_[key] = data;
                keys_ << key;
                        
                return true;
        }


        bool remove( const ResourceKey& key )
        {
                if ( key.isBuiltIn() ) 
                        return false;

                keys_.removeAll( key );
                return map_.remove( key );
        }
        
        const Resource operator[]( const ResourceKey& key ) const { return map_[key]; }
        const ResourceKey key( const Resource& data ) const { return map_[data]; }
        bool contains( const ResourceKey& key ) const { return map_.contains( key ); }

        const ResourceKeyList& keys() const { return keys_; }

//        QList<Resource> resources() const { return map_.values(); }
        
private:
        ResLib<Resource>() : map_(), keys_() {};
        ResLib<Resource>( const ResLib<Resource>& ) {}
        
        void insertBuiltIn( const ResourceKey& key, const Resource& data )
        {
                map_[key] = data;
                keys_ << key;
        }
        
        QMap<ResourceKey, Resource> map_;
        ResourceKeyList keys_;
        
};

class Stroke;
template<> class ResLib<Stroke>;

#endif
