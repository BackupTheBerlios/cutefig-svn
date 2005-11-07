 
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

#ifndef resourceio_h
#define resourceio_h

#include "resourcekey.h"

#include <QString>
#include <QHash>
#include <QTextStream>

#include <sstream>

class ResourceIO;

template<typename Resource> class ResLib;

/** \class ResourceIOFactory
 * \brief Used to lookup a suitable ResourceIO for a given Resource.
 *
 * In order to handle the IO of a certain resource a ResourceIO is
 * needed. The ResourceIOs are created on demand by "the"
 * ResourceIOFactory. Speaking of "the" ResourceIOFactory means that
 * there is only one instance of ResourceIOFactory for each type that
 * has a ResourceIO.
 * 
 * The only relevant function is ResourceIOFactory::getResourceIO()
 * which returns the suitable ResourceIO.
 *
 * Each type that has a ResourceIO needs to subclass from
 * ResourceIOFactory.  A subclass of the ResourceIOFactory has to
 * reimplement newResource() and to pass the key word of the resource
 * it handles. It is important that a static instance of each subclass
 * is created somewhere as a global variable so that the instance of
 * the subclass is created and registered. See stdresio.cc for
 * examples.  This design is described by a Singleton in the book
 * "Design Patterns" by the GoF.
 *
 * The instances subclasses of ResourceIOFactories are stored in the
 * static ResourceIOFactory::rIOFHash_. Each subclass registeres
 * itself by passing the keyword of the resource it handles to the
 * protected constructor.
 *
 * For a general overview of the resource system see \ref resource_system.
 */
class ResourceIOFactory
{
public:
        static ResourceIO* getResourceIO( const QString& keyWord );

        virtual ~ResourceIOFactory() {}
        
protected:
        ResourceIOFactory( const QString& kw )
        {
                rIOFHash_[kw] = this;
        }

        virtual ResourceIO* newResourceIO() = 0;

private:
        ResourceIOFactory( const ResourceIOFactory& ) {}
        
        static QHash<QString,ResourceIOFactory*> rIOFHash_;        
};

/** \class ResourceIO
 *
 * \brief Abstract base class of TResourceIO.
 *
 * A ResourceIO is used to handle the IO of resources. This is just an
 * abstract base class. The actual work is done by the templated
 * TResource. The only stuff handled by ResourceIO is the error
 * management by providing ResourceIO::failed() and
 * ResourceIO::errorString().
 *
 * For a general overview of the resource system see \ref resource_system.
 */
class ResourceIO 
{
public:
        ResourceIO()
                : failed_( false ),
                  errorString_()
        {}
        virtual ~ResourceIO() {}
        
        virtual int hashSum( const ResourceKey& key, bool* found ) const = 0;
        virtual bool parseResource( const QString& itemType, std::istream& is ) = 0;
        virtual void postProcessResource() = 0;
        virtual void pushResource( const ResourceKey& key ) const = 0;
        virtual void outputResource( const ResourceKey& key, QTextStream& stream ) = 0;
        
        bool failed() const { return failed_; }
        QString errorString() const { return errorString_; }
        
        friend class ResourceIOFactory;
        
protected:
        bool failed_;
        QString errorString_;

        QString keyWord_;

        const char* tr( const char* text ) { return text; }

private:
        ResourceIO( const ResourceIO& ) {}
};

/** \class TResourceIO
 *
 * \brief Implements the interface provided by ResourceIO.
 *
 * Besides implementing the pure virtual functions of ResourceIO it
 * contains the data member resource_ and resourceLib_ of which the
 * former is the Resource handled and the latter is a reference to
 * ResLib<Resource>::instance().
 *
 * Important functions are parseResource(), outputResourceBody() and
 * for some resource types postProcessResource(). These have to be
 * specislised for any type that are to be used as
 * resources.  
 *
 * For a general overview of the resource system see \ref resource_system.
 */
template<typename Resource>
class TResourceIO : public ResourceIO 
{
public:
        TResourceIO<Resource>()
                : ResourceIO(),
                  resourceLib_( ResLib<Resource>::instance() )
        {}

        //! Looks up the hash sum of the resource key in the resourceLib_.
        int hashSum( const ResourceKey& key, bool* found = 0 ) const 
        {
                return resourceLib_.hashSum( key, found );
        }

        //! \b Needs to be specialised by all types
        /*! Is called by the Parser when a resource is to be parsed.
         * \param itemType is a reference to Parser::itemType_.
         * \param is is a reference to Parser::stream_.
         *
         * The called several times until the Parser finds
         * resource_end as a keyword. Each time one line is
         * passed. The first word in itemType and the rest in is.  If
         * the line passed is the first line describing the resource
         * itemType is QString::null.
         *
         * If the resource type can be several lines long \i true has to
         * be returned. If it is \b always only one line long \i false
         * has to be returned.
         */
        virtual bool parseResource( const QString& itemType, std::istream& is );

        //! Is to be specialised by some types
        virtual void postProcessResource() {}

        //! \b Needs to be specialised
        void outputResource( const ResourceKey& key, QTextStream& stream );

        //! adds the resource to the resourceLib after having successfully parsed it.
        void pushResource( const ResourceKey& key ) const 
        {
                resourceLib_.insert( key, resource_ );
        }

protected:
        //! \b Needs to be specislised by all types
        virtual void outputResourceBody( QTextStream& ) {}
        
private:
        ResLib<Resource>& resourceLib_;

        Resource resource_;
};

template<typename Resource>
void TResourceIO<Resource>::outputResource( const ResourceKey& key, QTextStream& stream )
{
        resource_ = resourceLib_[key];
        
        stream << "resource "
               << keyWord_ << " "
               << key.keyString() << " "
               << hashSum( key ) << " ";

        outputResourceBody( stream );

        stream << "resource_end\n";
}

#endif
