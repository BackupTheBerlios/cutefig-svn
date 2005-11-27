 
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

/*!
 * \page resource_system The resource system
 *
 *
 * \section resource_rationale Rationale
 * 
 * The CuteFig has the concept of resources and resource libraries. A
 * resource can be any data that is used in a figure. Resources of a
 * certain type can be stored in a resource library ResLib. A resource
 * can be accessed out of the ResLib by a ResourceKey.
 *
 * The purpose of resources is to store data that is used in several
 * figures or in one figure several times. Examples of those types are
 * Dashes a Gradient.
 *
 * Resources can be either built into the CuteFig, stored in a local
 * library or brought up by a figure file. Builtin resources are
 * garantueed to be available identically on all installations of the
 * CuteFig of the same version. So they cannot be changed or removed
 * from the ResLib. The way resources are known is stored internally
 * in the ResourceKey by the peivate member ResourceKey::flags_. The
 * function ResourceKey::isBuiltIn() can be used to determine whether
 * a resource is a built in one.
 *
 * In order to be able to read all non-builtin resources have to be
 * stored in the figure file. This is known by calling
 * ResourceKey::isToBeSaved(). In the figure file
 * ResourceKey::keyString() is stored to give the resource its
 * name. To be able to compare the resource with a resource of the
 * local library that has the same name a hash sum is stored by
 * complex resources. Therefore every resource data type is required
 * to implement the function unsigned int qHash( const Resource& ).
 * In case the function returns 0, the resources are compared directly
 * using the == operator if not the hashes are compared. The usual way
 * qHash() is implemented is to stream all relevant data of the
 * resource into a QByteArray and then call qHash( const QByteArray&
 * ). The hash sum of a resource is stored in the figure file and in
 * the local library so that those to hash sums can be compared. If
 * the hash sums are identical the resource parsing is skipped and the
 * resource found in the library is used.
 *
 * The class ResLib stores resources of a certain type. The Parser
 * comes with the private method Parser::parseResource() to parse a
 * resource out of a figure file or the library. CFigOutput has a
 * private method CFigOutput::outputResources() to write resources
 * into a figure file or the library. The actual IO-processing of
 * resources is done by the class TResourceIO which is a subclass of
 * ResrouceIO. The suitable instance of TResourceIO is created by a
 * subclass of ResourceIOFactory. Therefore each data type that is
 * used as a resource must be assigned a keyword. This keyword is used
 * in the figure file to determine the suitable ResourceIO.
 *
 * TResourceIO is templated with the template parameter being the data
 * type of the resource. In order to be able to process resources the
 * methods TResourceIO::parseResource() and
 * TResourceIO::outputResourceBody() have to be specialised. See
 * stdresio.cc for examples.
 *
 * As well as in the figure file as well as in the resource library
 * file resources are stored in the following syntax:
 *
 * resource <typename> <resourceKey> <hashsum> <em>(initial resource data)</em>           <br>
 * <em>one or several lines of additional resource data</em>                              <br>
 * resource_end
 *
 * If there is no line of additional resource data resource_end is the
 * last token of the first line.
 * 
 * All classes that handle resouces generically are implemented using
 * templates. The class methods of those classes that are not
 * implementable generically have to be specialised.
 *
 * \section resourcesystem_key_classes Key Classes
 *
 * The following classes are important for the resource system:
 *
 * In the kernel:
 *     - ResourceKey -- the key to resources in ResLib
 *     - ResLib <T> -- the resource library for a certain data type
 *     - ResourceUser <T> -- a user of a resource. Acts a little bit like a
 *       reference counter.
 *     - ResourceIO -- handles the IO of resourced data
 *     - ResLibInit -- fills the ResLibs with the builtIn data on application
 *       startup.
 *
 * Widgets:
 *     - ResourceButton <T> -- a button that shows the resource data and
 *       invokes dialog to edit the resource data.
 *     - ResourceDemo <T> -- a widget that shows the resource data
 *     - ReslibEditor <T> -- edits the ResLib <T>
 *     - ResourceIconEngine <T> -- used to implement QIcons to show resource
 *       data.
 *     - ResourceModel <T> -- adapts the ResLib <T> to Qt's MVC architecture
 *      
 */

#ifndef resourcekey_h
#define resourcekey_h

#include <QString>
#include <QHash>

#include <QDebug>

class ResourceKey;

uint qHash( const ResourceKey& key );
bool operator== ( const ResourceKey& k1, const ResourceKey& k2 );
bool operator<  ( const ResourceKey& k1, const ResourceKey& k2 );


//! Serves as the key to access a certain resource.
/*!
 * A ResourceKey is the key to access a resource stored in a ResLib by
 * ResLib::operator[const ResourceKey&]. A ResourceKey contains a
 * QString keyString_ that is stored into the figure file and
 * displayed to the user. The other data that is bound to a
 * ResourceKey are the ResourceKey::Flags flags_. These are to keep
 * track of the way a resource is stored. It can be either BuiltIn,
 * InLib, or InFig. In the future the CuteFig will be able to handle
 * serveral instances of Figure at once. Then every figure will be
 * numbered a subsequent number this number will be ored onto the
 * flags_ in order to be able to have different resources with an
 * identical keyStrings_ in the several instances of Figure.
 *
 * For a general overview of the resource system see \ref resource_system.
 */
class ResourceKey
{
public:
        //! represent the way a Resource is stored
        enum Flags { Invalid = 0x00, BuiltIn = 0x01, InLib = 0x02, InFig = 0x04 };
        
        friend uint qHash( const ResourceKey& key );
        friend bool operator== ( const ResourceKey& k1, const ResourceKey& k2 );
        friend bool operator<  ( const ResourceKey& k1, const ResourceKey& k2 );

        ResourceKey() : keyString_(), flags_( Invalid ) {}
        ResourceKey( const QString& ks, Flags flags ) : keyString_( ks ), flags_( flags ) {}
        ResourceKey( const ResourceKey& o ) : keyString_( o.keyString_ ), flags_( o.flags_ ) {}
        
        const QString& keyString() const { return keyString_; }

        //! true if the Resource is built in
        bool isBuiltIn() const { return flags_ & BuiltIn; }

        //! true if the Resource is to be saved in to the figure file
        bool isToBeSaved() const { return flags_ > BuiltIn; }

        bool isValid() const { return flags_ != Invalid; }

        static ResourceKey builtIn( const QString& ks ) { return ResourceKey( ks, BuiltIn ); }
        static ResourceKey inFig( const QString& ks ) { return ResourceKey( ks, InFig ); }
        static ResourceKey inLib( const QString& ks ) { return ResourceKey( ks, InLib ); }

        //! used if the user changed the name of a resource
        static ResourceKey newName( const QString& ks, const ResourceKey& other )
        { return ResourceKey( ks, other.flags_ ); }
        
        friend QDebug operator<< ( QDebug, const ResourceKey& );
        
private:
        QString keyString_;
        Flags flags_;
};

//! needed for lookups of QMap
inline bool operator== ( const ResourceKey& k1, const ResourceKey& k2 )
{
        return k1.keyString_ == k2.keyString_ && k1.flags_ == k2.flags_;
}

//! needed for sorting ResourceKey in QMap
inline bool operator< ( const ResourceKey& k1, const ResourceKey& k2 )
{
        if ( k1.flags_ < k2.flags_ )
                return true;

        if ( k1.flags_ > k2.flags_ )
                return false;

        return k1.keyString_ < k2.keyString_;
}

//! obsolete, was needed for lookups when ResLib used QHash
inline uint qHash( const ResourceKey& key )
{
        return qHash( qHash( key.keyString_ ) ^ key.flags_ );
}

inline QDebug operator<< ( QDebug dbg, const ResourceKey& key ) 
{
        dbg << key.keyString_ << key.flags_;
        return dbg;
}

#endif
