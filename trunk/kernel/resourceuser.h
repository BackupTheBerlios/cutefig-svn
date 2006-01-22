 
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

#ifndef resourceuser_h
#define resourceuser_h

#include "resourcekey.h"

#include <QDebug>

//! Abstract base class for ResourceUser
/** Used to put several different ResourceUsers into one container
 *  class. This is useful for example to make a
 *  QList<AbstractResourceUser> of all the Resources a Figure
 *  uses. Therefore DrawObject::resourceUsers() is used.
 *
 *  Most of its functions are pure virtual and they are reimplemented
 *  in ResourceUser. Except for ResourceUser, AbstractResourceUser is
 *  not meant to be subclassed.
 * 
 */
class AbstractResourceUser
{
public:
        AbstractResourceUser() : key_() {}
        AbstractResourceUser( const  AbstractResourceUser& o ) : key_( o.key_ ) {}

        virtual ~AbstractResourceUser() {}

        //! used to copy the ResourceUser
        virtual AbstractResourceUser* clone() const = 0;

        //! returns the key of the Resource tracked
        const ResourceKey& key() const { return key_; }

        virtual void setResource( const ResourceKey& key ) = 0;

        virtual void releaseResource() = 0;
        virtual void reclaimResource() = 0;

        virtual const QString resourceName() const = 0;

        //! returns the resource data
        template<typename Resource> const Resource& resource() const;
        
protected:
        //! the key of the Resource tracked.
        ResourceKey key_;
};

#include "reslib.h"

//! Acts a little bit like a refcount for a Resource
/** The \ref resource_system of the CuteFig, arises the following
 *  problem: What happens if a Resource, that is in use in the Figure
 *  is removed from the ResLib?
 *
 *  Therefore the Resource should be kept in a ResourceUser. The
 *  ResourceUser uses ResLib::assignResource() and
 *  ResLib::unassignResource() to let the ResLib keep track of the
 *  resources that are in use.
 *
 *  If a DrawObject is removed from the figure all the Resources that
 *  it uses are unassigned. If one of these resources is then removed
 *  from the ResLib the Resource is still kept in the ResourceUser. So
 *  it can be readded to the ResLib, when the DrawObject is readded
 *  e.g. by PasteAction or by DeleteAction::unexecute().
 *
 *  A ResourceUser can also contain a Resource that is not in a
 *  ResLib, i.e. that has no ResourceKey. So far that is only used by
 *  Stroke in case a Stroke contains a color.
 */
template<typename Resource> class ResourceUser : public AbstractResourceUser
{
public:
        ResourceUser<Resource>()
                : AbstractResourceUser(),
                  data_(),
                  p_data_( 0 )
        {}

        ResourceUser<Resource>( const Resource& r )
                : AbstractResourceUser(),
                  data_( r ),
                  p_data_( 0 )
        {}
        
        ResourceUser<Resource>( const ResourceUser<Resource>& other );

        ~ResourceUser();
        
        AbstractResourceUser* clone() const { return new ResourceUser<Resource>( *this ); }

        //! returns a reference to the Resource data.
        const Resource& data() const;

        //! sets the resource to data.
        void setResource( const Resource& data );

        //! stets the resource to the Resource that is behind the key
        void setResource( const ResourceKey& key );

        //! releases the resource of a removed DrawObject
        void releaseResource();

        //! readds the resource of a readded DrawObject
        void reclaimResource();

        //! returns the keyword of the Resource
        const QString resourceName() const { return Res::resourceName<Resource>(); }
        
private:
        //! makes the ResLib remove the user from the userlist.
        void unassignResource();

        //! the resource data in case the is not in the ResLib
        Resource data_;

        //! a pointer to the ResourceData in case the Resource is in the ResLib
        const Resource* p_data_;

        //! the ResLib::instance()
        static ResLib<Resource>& resLib_;
};

template<typename Resource>
ResLib<Resource>& ResourceUser<Resource>::resLib_ = ResLib<Resource>::instance();

/** Needs to add itself to the userlist in case the key is valid.
 */
template<typename Resource>
ResourceUser<Resource>::ResourceUser<Resource>( const ResourceUser<Resource>& other )
        : AbstractResourceUser( other ),
          data_( other.data_ ),
          p_data_( other.p_data_ )
{
        if ( key_.isValid() )
                resLib_.assignResource( key_, this );
}

/** The user has to be removed from the user list on destruction.
 */
template<typename Resource>
ResourceUser<Resource>::~ResourceUser()
{
        unassignResource();
}

/** Returns the "hard stored" data in case the key is *
 *  invalid. Otherwise a pointer to the Resource instance in the ResLib
 *  is returned.
 */
template<typename Resource> const Resource& ResourceUser<Resource>::data() const
{
        if ( key_.isValid() )
                return *p_data_;
        else
                return data_;
}

/** unassigns the Resource first.
 */
template<typename Resource>
void ResourceUser<Resource>::setResource( const Resource& data )
{
        unassignResource();
        key_ = ResourceKey();
        data_ = data;
}

/** Looks up the resource in ResLib by assigning it. If the Resource
 *  could be looked up the old one is unassigned before setting the
 *  data.
 */
template<typename Resource>
void ResourceUser<Resource>::setResource( const ResourceKey& key )
{
        key_ = key;
        const Resource* r = resLib_.assignResource( key_, this );

        if ( r || !key_.isValid() ) {
                unassignResource();
                p_data_ = r;
        }
}

/** The data is copied out of the ResLib so that it can be readded
 *  later.
 */
template<typename Resource>
void ResourceUser<Resource>::releaseResource()
{
        if ( !key_.isValid() )
                return;

        data_ = *p_data_;
        unassignResource();
}

/** Of course the user is added to the ResLibs userlist.
 */
template<typename Resource>
void ResourceUser<Resource>::reclaimResource()
{
        if ( !key_.isValid() )
                return;

        if ( !resLib_.contains( key_ ) )
                resLib_.insert( key_, data_ );
        p_data_ = resLib_.assignResource( key_, this );
}       

template<typename Resource>
void ResourceUser<Resource>::unassignResource()
{
        if ( key_.isValid() ) 
                resLib_.unassignResource( key_, this );
}

/** Auxilliary function. Mainly used to save typing and to avoid long
 *  statemants and thus enhance readability of code.
 *
 *  Compare:
 *      - Type foo = static_cast<ResourceUser<Type>*>( abtractReourceUser )->data();
 *      - Type foo = abstractResourceUser->resource<Type>();
 */ 
template<typename Resource>
const Resource& AbstractResourceUser::resource() const
{
        return static_cast<const ResourceUser<Resource>*>( this )->data();
}


#endif
