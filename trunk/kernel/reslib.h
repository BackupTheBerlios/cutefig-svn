 
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

#include "typedefs.h"
#include "resourcekey.h"

#include <QDebug>

// The following two lines are an ugly hack to make the thing compile
// using g++-3.3. The will be dropped as soon as Gentoo Linux switches
// to g++-4.0 :)
template<typename Resource> class ResLib;
#include "strokelib.h"

class ResLibInit;

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
        
        const Resource operator[]( const ResourceKey& key ) const { return hash_[key]; }
        Resource& operator[]( const ResourceKey& key ) { return hash_[key]; }
        const ResourceKey key( const Resource& data ) const { return hash_[data]; }

        bool contains( const ResourceKey& key ) const { return hash_.contains( key ); }

        int count() const { return hash_.count(); }
        ResourceKey at( int i ) const { return hash_.keys().at ( i ); }
        int indexOf( const ResourceKey& key ) const { return hash_.keys().indexOf( key ); }

        const ResourceKeyList keys() const { return hash_.keys(); }

        
//        QList<Resource> resources() const { return hash_.values(); }
        
private:
        ResLib<Resource>() : hash_() {}; //!< Private to enforce singularity.
        ResLib<Resource>( const ResLib<Resource>& ) {} //! just to avoid copying the instance

        void insertBuiltIn( const ResourceKey& key, const Resource& data )
        //!<< inserts a builtIn resource. It is accessible by friends. 
        {
                hash_[key] = data;
        }
        
        QHash<ResourceKey, Resource> hash_; //!< resolves a ResourceKey to a Resource
        QHash<ResourceKey, int> hashSums_; //!< resolves a ResourceKey to the hash sum        
};


template<> class ResLib<Stroke>;

/** It therefore first checks whether the resource is not
 *  builtIn. Also the hash sum is calculated.
 */
template<typename Resource>
bool ResLib<Resource>::insert( const ResourceKey& key, const Resource& data )
{
        if ( key.isBuiltIn() )
                return false;

        hash_[key] = data;

        int hashsum = qHash( data );
                
        if ( hashsum )
                hashSums_[key] = hashsum;
                        
        return true;
}

/** It therefore first checks whether the resource is not
 *  builtIn. 
 */
template<typename Resource>
bool ResLib<Resource>::remove( const ResourceKey& key )
{
        if ( key.isBuiltIn() ) 
                return false;

        hashSums_.remove( key );
        return hash_.remove( key );
}

/** If found is nonnull it is used to tell whether the resource has
 * been found in the lib or not.
 */
template<typename Resource>
int ResLib<Resource>::hashSum( const ResourceKey& key, bool* found ) const
{
        bool f = false;
        int result = 0;
                
        if ( hash_.contains( key ) ) {
                if ( hashSums_.contains( key ) )
                        result = hashSums_[key];
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
                
        if ( hash_.contains( key ) ) {
                newsum = qHash( hash_[key] );
                f = true;
        }

        if ( found )
                *found = f;

        return newsum;
}
        

#endif
