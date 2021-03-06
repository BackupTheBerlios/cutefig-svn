 
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

//! A small namespace to avoid name colision with a globally visible function
// namespace Res
// {
//         //! returns the key word of Resource used in the figure file
//         /*! The actual functions ar to be sepecialised.
//          */
//         template<typename Resource> const QString resourceName();
// }

#include "typedefs.h"
#include "resourcekey.h"
#include "resourceuser.h"
#include "resourceio.h"

#include <ostream>

#include <QDebug>

class AbstractResourceUser;
class ResourceIO;
//! Abstract base class of ResLib
/*! Used to put all the different ResLibs of different Resource type in
 *  one container class.
 */
class AbstractResLib
{
public:
        virtual ~AbstractResLib() {}
        
        virtual void unassignResource( const ResourceKey& key, AbstractResourceUser* u ) = 0;

        virtual void save( ResourceIO* rio, QTextStream& ts ) = 0;

        virtual bool contains( const ResourceKey& key ) const =0;

        
protected:
        AbstractResLib() {};

        bool containsInFigOrLib( ResourceKey& key );

        ResourceKey findKeyImpl( const QString& keyString );
        
private:
        AbstractResLib( const AbstractResLib& );
        AbstractResLib& operator=( const AbstractResLib& );
};

//! stores resources of the type Resource
/*!
 * Resouces of a certain type T are accessible by the
 * ResLib<T>. Speaking of "the" ResLib<T> means that there is only one
 * instance of a ResLib for every type which is accessible by the
 * static method ResLib<T>::instance(). The singularity is enforced by
 * the Singleton design pattern defined in the Book "Design Patterns"
 * by the GoF.
 *
 * For a general overview of the resource system see \ref resource_system.
 */
template<class Resource> class ResLib : public AbstractResLib
{
public: 
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

        // **obsolete** Recalculates the hash sum represented by key.
        // int recalcHashSum( const ResourceKey& key, bool* found = 0 ) const;

        //! gives access to the Resource behind key.
        const Resource& operator[]( const ResourceKey& key ) const;

        //! changes the Resource behind key, if it is in the ResLib
        bool setResource( const ResourceKey& key, const Resource& resource );

        //! true if ResLib contains a resource of the key key
        bool contains( const ResourceKey& key ) const { return map_.contains( key ); }
                
        //! true if the Resource of key is used by a ResourceUser
        bool isBeingUsed( const ResourceKey& key ) const { return !map_[key]->users.isEmpty(); }

        //! the number of items in the ResLib
        int count() const { return map_.count(); }

        //! the Resource at the i'th position
        ResourceKey at( int i ) const { return map_.keys().at( i ); }

        //! the position of the Resource of key
        int indexOf( const ResourceKey& key ) const { return map_.keys().indexOf( key ); }

        //! change the key name of the Resource without changing the resource itself
        bool changeKeyName( const ResourceKey& oldKey, const ResourceKey& newKey );

        //! returns a list of all the keys in ResLib
        const ResourceKeyList keys() const { return map_.keys(); }

        //! adds a new ResourceUser to the list of resource users
        const Resource* assignResource( ResourceKey& key, AbstractResourceUser* u );

        //! removes a ResourceUser of the list of resource users
        void unassignResource( const ResourceKey& key, AbstractResourceUser* u );

        void save( ResourceIO* rio, QTextStream& ts );

        static ResourceKey findKey( const QString& ks ) { return instance().findKeyImpl( ks ); }

        static ResourceKey defaultKey();
        static const Resource& defaultResource() { return instance()[defaultKey()]; }

        static const QString resourceName();
        
private:
        /*! Private to enforce singularity according to the Singleton
         *  design pattern.
         */
        ResLib<Resource>()
                : AbstractResLib(),
                  map_() 
        {
                init();
        }

        //! just to avoid copying the instance
        ResLib<Resource>( const ResLib<Resource>& ); 

        //! Initialises the ResLib when the Singleton instance constructed.
        /*! The function is not defined generically, so it has to be
         *  specialised for every type ResLib is to be instanciated
         *  for. Its normal job is to fill the ResLib with the builtIn
         *  Resources.
         */
        void init();
                
        //! inserts a builtIn resource. 
        void insertBuiltIn( const QString& key, const Resource& data )
        {
                map_[ResourceKey::builtIn(key)] = new ResourceData( data );
        }

        class ResourceData;

        //! a default constructed instance of Resource
        //static Resource& dummyResource();
        
        //! resolves a ResourceKey to a Resource
        /*! Note that the key is resolved to a \e pointer to a
         *  ResourceData instance. The reason for that is, that we can
         *  use const T QMap::operator[key] const for the lookup which
         *  returns by value.
         */
        QMap<ResourceKey, ResourceData*> map_;
};


template<> class ResLib<Stroke>;


/*! This is to be returned if a resource is requested by a key that is
 *  not in the ResLib.
 */
// template<typename Resource>
// Resource& ResLib<Resource>::dummyResource()
// {
//         static Resource r;
//         return r;
// }


/*! It therefore first checks whether the resource is not
 *  builtIn.
 */
template<typename Resource>
bool ResLib<Resource>::insert( const ResourceKey& key, const Resource& data )
{
        if ( key.isBuiltIn() || contains( key ) )
                return false;

        map_[key] = new ResourceData( data );
                        
        return true;
}

/*! It therefore first checks whether the resource is not builtIn. The
 *  instance of ResourceData is deleted before the entry is removed.
 */
template<typename Resource>
bool ResLib<Resource>::remove( const ResourceKey& key )
{
        if ( key.isBuiltIn() || isBeingUsed( key ) || !map_.contains( key ) ) 
                return false;

        delete map_[key];
        return map_.remove( key );
}

/*! If found is nonnull it is used to tell whether the resource has
 * been found in the lib or not.
 */
template<typename Resource>
int ResLib<Resource>::hashSum( const ResourceKey& key, bool* found ) const
{
        bool f = false;
        int result = 0;
                
        if ( map_.contains( key ) ) {
                result = map_[key]->hashSum();
                f = true;
        }

        if ( found )
                *found = f;

        return result;
}

/* ** obsolete ** If found is nonnull it is used to tell whether the resource has
 * been found in the lib or not.
 */
// template<typename Resource>
// int ResLib<Resource>::recalcHashSum( const ResourceKey& key, bool* found ) const
// {
//         bool f = false;
//         int newsum = 0;
                
//         if ( map_.contains( key ) ) {
//                 newsum = qHash( map_[key] );
//                 f = true;
//         }

//         if ( found )
//                 *found = f;

//         return newsum;
// }

/*! returns the default constructed dummyResource() in case the key can
 *  not be looked up
 */
template<typename Resource>
const Resource& ResLib<Resource>::operator[]( const ResourceKey& key ) const
{
        if ( map_.contains( key ) ) 
                return map_[key]->data();
        else 
                return defaultResource();
}

template<typename Resource>
bool ResLib<Resource>::setResource( const ResourceKey& key, const Resource& res ) 
{
        if ( !key.isBuiltIn() && map_.contains( key ) ) {
                map_[key]->setData( res );
                return true;
        }

        return false;
}


/*! All users of the resource are informed of the name change.
 */
template<typename Resource>
bool ResLib<Resource>::changeKeyName( const ResourceKey& oldKey, const ResourceKey& newKey )
{
        if ( !contains( oldKey ) || contains( newKey ) )
                return false;
        
        ResourceData* d = map_[oldKey];
        map_[newKey] = d;
        foreach ( AbstractResourceUser* u, d->users )
                u->setResource( newKey );
        
        map_.remove( oldKey );

        return true;
}

/*! Does nothing if the key could not be looked up or if the user is
 *  already in the userlist.
 */
template<typename Resource>
const Resource* ResLib<Resource>::assignResource( ResourceKey& key, AbstractResourceUser* u )
{
        if ( !containsInFigOrLib( key ) )
                return 0;
        
        ResourceData* d = map_[key];
        if ( !d->users.contains( u ) )
                d->users << u;
        
        return &d->data();
}

/*! does nothing if the key could not be looked up
 */
template<typename Resource>
void ResLib<Resource>::unassignResource( const ResourceKey& key, AbstractResourceUser* u ) 
{
        if ( !contains( key ) )
                return;

        map_[key]->users.removeAll( u );
}

template<typename Resource>
void ResLib<Resource>::save( ResourceIO* rIO, QTextStream& ts )
{
        const ResourceKeyList kl = map_.keys();
        foreach ( const ResourceKey key, kl )
                if ( key.isToBeSaved() )
                        rIO->outputResource( key, ts );
}


// template<typename Resource>
// ResourceKey ResLib<Resource>::findKey( const QString& keyString )

//! Contains the data of the Resource, the hash sum and the user list.
/*! and keeps all consistent
 */
template<typename Resource>
class ResLib<Resource>::ResourceData
{
public:
        ResourceData()  : users(), data_(), hashSum_() {}
        ResourceData( const Resource& d ) : users(), data_( d ), hashSum_()
        {
                setData( d );
        }

        const Resource& data() const { return data_; }

        unsigned int hashSum() const { return hashSum_; }
        
        QList<AbstractResourceUser*> users;

        void setData( const Resource& d );

private:
        ResourceData( const ResourceData& );
        ResourceData& operator=( const ResourceData& );
        
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
