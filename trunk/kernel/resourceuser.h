 
/*****************************************************************************
**
**  $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2005 Johannes Mueller, johmue@users.sourceforge.net
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

#ifndef resourceuser_h
#define resourceuser_h

#include "resourcekey.h"
#include "reslib.h"

#include <QDebug>

template<typename Resource> class ResourceUser
{
public:
        ResourceUser<Resource>()
                : data_(),
                  p_data_( 0 ),
                  key_()
        {}

        ResourceUser<Resource>( const Resource& r )
                : data_( r ),
                  p_data_( 0 ),
                  key_()
        {}
        
        ResourceUser<Resource>( const ResourceUser<Resource>& other );

        ~ResourceUser();

        const Resource& data() const;
//        Resource& data();

        const ResourceKey& key() const { return key_; }
        
        void setResource( const Resource& data );
        void setResource( const ResourceKey& resource );

        void nameChanged( const ResourceKey& newKey ) { key_ = newKey; }
        
private:
        void unassignResource();
        Resource data_;
        const Resource* p_data_;
        
        ResourceKey key_;

        static ResLib<Resource>& resLib_;
};

template<typename Resource>
ResLib<Resource>& ResourceUser<Resource>::resLib_ = ResLib<Resource>::instance();


template<typename Resource>
ResourceUser<Resource>::ResourceUser<Resource>( const ResourceUser<Resource>& other )
        : data_( other.data_ ),
          p_data_( other.p_data_ ),
          key_( other.key_ )
{
        if ( key_.isValid() )
                resLib_.assignResource( key_, this );
}


template<typename Resource>
ResourceUser<Resource>::~ResourceUser()
{
        unassignResource();
}

template<typename Resource>
const Resource& ResourceUser<Resource>::data() const
{
        if ( !key_.isValid() )
                return data_;
        else
                return *p_data_;
}

// template<typename Resource>
// const Resource& ResourceUser<Resource>::data()
// {
//         if ( !key_.isValid() )
//                 return data_;
//         else
//                 return *p_data_;
// }

template<typename Resource>
void ResourceUser<Resource>::setResource( const Resource& data )
{
        unassignResource();
        key_ = ResourceKey();
        data_ = data;
}

template<typename Resource>
void ResourceUser<Resource>::setResource( const ResourceKey& key )
{
        const Resource* r = resLib_.assignResource( key, this );

        if ( r ) {
                unassignResource();
                key_ = key;
                p_data_ = r;
        }
}

template<typename Resource>
void ResourceUser<Resource>::unassignResource()
{
        if ( key_.isValid() ) 
                resLib_.unassignResource( key_, this );
}

#endif
