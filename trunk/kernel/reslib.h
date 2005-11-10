 
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
#include "resourceuser.h"
#include <QDebug>

// The following two lines are an ugly hack to make the thing compile
// using g++-3.3. The will be dropped as soon as Gentoo Linux switches
// to g++-4.0 :)
// template<typename Resource> class ResLib;
// #include "strokelib.h"

class ResLibInit;
template<typename Resource> class ResourceUser;

/** \class ResLib<Resource>
 *
 * \brief stores resources of the type Resource
 *
 * Resouces of a certain type T are accessible by the
 * ResLib<T>. Speaking of "the" ResLib<T> means that there is only one
 * instance of a ResLib for every type which is accessible by the
 * static method ResLib::instance(). The singularity is enforced by
 * the Singleton design pattern defined in the Book "Design Patterns"
 * by the GoF.
 *
 * For a general overview of the resource system see \ref resource_system.
 */

template<class Resource> class ResLib
{
        typedef ResourceUser<Resource> User;
        
public:
        friend class ResLibInit;
        //!< fills the ResLib with the builtin resources on application startup.
        friend void ResLib<Stroke>::insertBuiltIn( const ResourceKey&, const Stroke& );
//        friend class ResLib<Stroke>;
        
        //! returns the instance of the ResLib.
        static ResLib<Resource>& instance() 
        {
                static ResLib<Resource> inst;
                return inst;
        };

        //! Inserts data into the ResLib with the key
        bool insert( const ResourceKey& key, const Resource& data );
        //! Removes the resource represented by key.
        bool remove( const ResourceKey& key );

        //! Returns the hash sum of the resource represented by key.
        int hashSum( const ResourceKey& key, bool* found = 0 ) const;
        //! Recalculates the hash sum represented by key.
        int recalcHashSum( const ResourceKey& key, bool* found = 0 ) const;
        
        const Resource& operator[]( const ResourceKey& key );
        
        void setResource( const ResourceKey& key, const Resource& resource );
        
//        const ResourceKey key( const Resource& data ) const { return map_[data]; }

        bool contains( const ResourceKey& key ) const { return map_.contains( key ); }

        bool isBeingUsed( const ResourceKey& key ) const { return !map_[key].users.isEmpty(); }
        
        int count() const { return map_.count(); }
        ResourceKey at( int i ) const { return map_.keys().at ( i ); }
        int indexOf( const ResourceKey& key ) const { return map_.keys().indexOf( key ); }

        void changeKeyName( const ResourceKey& oldKey, const ResourceKey& newKey );
        
        const ResourceKeyList keys() const { return map_.keys(); }

        const Resource* assignResource( const ResourceKey& key, User* u );
        void unassignResource( const ResourceKey& key, User* u );
        
//        QList<Resource> resources() const { return map_.values(); }
        
private:
        ResLib<Resource>() : map_() {}; //!< Private to enforce singularity.
        ResLib<Resource>( const ResLib<Resource>& ) {} //! just to avoid copying the instance

        void insertBuiltIn( const ResourceKey& key, const Resource& data )
        //!<< inserts a builtIn resource. It is accessible by friends. 
        {
                map_[key] = data;
        }

        class ResourceData;

        static Resource dummyResource_;
        
        QMap<ResourceKey, ResourceData> map_; //!< resolves a ResourceKey to a Resource
//        QHash<ResourceKey, int> hashSums_; //!< resolves a ResourceKey to the hash sum        
};


template<> class ResLib<Stroke>;

template<typename Resource>
Resource ResLib<Resource>::dummyResource_ = Resource();

/** It therefore first checks whether the resource is not
 *  builtIn. Also the hash sum is calculated.
 */
template<typename Resource>
bool ResLib<Resource>::insert( const ResourceKey& key, const Resource& data )
{
        if ( key.isBuiltIn() || contains( key ) )
                return false;

        map_[key] = data;
                        
        return true;
}

/** It therefore first checks whether the resource is not
 *  builtIn. 
 */
template<typename Resource>
bool ResLib<Resource>::remove( const ResourceKey& key )
{
        if ( key.isBuiltIn() || !isBeingUsed( key ) ) 
                return false;

        return map_.remove( key );
}

/** If found is nonnull it is used to tell whether the resource has
 * been found in the lib or not.
 */
template<typename Resource>
int ResLib<Resource>::hashSum( const ResourceKey& key, bool* found ) const
{
        bool f = false;
        int result = 0;
                
        if ( map_.contains( key ) ) {
                result = map_[key].hashSum();
                f = true;
        }

        if ( found )
                *found = f;

        return result;
}

/** If found is nonnull it is used to tell whether the resource has
 * been found in the lib or not.
 */
template<typename Resource>
int ResLib<Resource>::recalcHashSum( const ResourceKey& key, bool* found ) const
{
        bool f = false;
        int newsum = 0;
                
        if ( map_.contains( key ) ) {
                newsum = qHash( map_[key] );
                f = true;
        }

        if ( found )
                *found = f;

        return newsum;
}

template<typename Resource>
const Resource& ResLib<Resource>::operator[]( const ResourceKey& key )
{
        if ( map_.contains( key ) )
                return map_[key].data();
        else {
                return dummyResource_;
        }
}

template<typename Resource>
void ResLib<Resource>::changeKeyName( const ResourceKey& oldKey, const ResourceKey& newKey )
{
        if ( !contains( oldKey ) || contains( newKey ) )
                return;

        ResourceData& d = map_[oldKey];
        foreach ( User* u, d.users )
                u->nameChanged( newKey );

        map_.remove( oldKey );
        map_[newKey] = d;
}


template<typename Resource>
const Resource* ResLib<Resource>::assignResource( const ResourceKey& key, User* u )
{
        if ( !contains( key ) )
                return 0;
        
        ResourceData& d = map_[key];
        if ( !d.users.contains( u ) )
                d.users << u;
        
        return &d.data();
}

template<typename Resource>
void ResLib<Resource>::unassignResource( const ResourceKey& key, User* u ) 
{
        if ( !contains( key ) )
                return;

        map_[key].users.removeAll( u );
}


template<typename Resource>
class ResLib<Resource>::ResourceData
{
public:
        ResourceData()  : users(), data_(), hashSum_() {}
        ResourceData( const Resource& d ) : users(), data_(), hashSum_()
        {
                setData( d );
        }

        const Resource& data() const { return data_; }

        unsigned int hashSum() const { return hashSum_; }
        
        QList<User*> users;

private:
        void setData( const Resource& d );
        Resource data_;
        unsigned int hashSum_;
};

template<typename Resource>
void ResLib<Resource>::ResourceData::setData( const Resource& d ) 
{
        data_ = d;
        hashSum_ = qHash( d );
}



#endif
