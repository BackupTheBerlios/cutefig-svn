 
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

#ifndef objecthandler_h
#define objecthandler_h

#include "typedefs.h"

#include <QHash>
#include <QString>
#include <QTextStream>

class DrawObjectIO;
class DrawObject;
class Figure;
class ObjectGUIHandler;

namespace Initialiser
{
        template<typename T> class AutoHash;
}


//! Resolves a keyword to a registered DrawObject
/*! There are only static routines in the public interface. Those
 *  routines take the keyword of an object and react to it
 *  appropriately.
 *
 *  ObjectHandler itself is an abstract base class. Users are to use
 *  the templated derived class TObjectHandler <ObjectType>.
 *
 *  Up to now the class is meant to be used for two ways.
 *      - to parse the DrawObject (called by Parser)
 *      - to return an ObjectGUIHandler.
 *
 *  For the lookup a static Initialiser::AutoHash is used. Read the
 *  section about the Singleton pattern in the book by the GoF.
 *
 *  The design of returning the ObjectGUIHAndler is somewhat ugly. The
 *  problem is that the commandline tools have to be able to use
 *  getDrawObject() without being required to know about GUI related
 *  stuff.
 */
class ObjectHandler
{
public:
        //! returns a DrawObject for keyword.
        static DrawObject* getDrawObject( const QString& keyword, QTextStream& is, Figure* f );

        //! returns an appropriate ObjectGUIHandler
        static ObjectGUIHandler* guiHandler( const QString& keyWord );
        
        virtual ~ObjectHandler() {}

        //! registers a ObjectGUIHandler for a keyword.
        static void registerGUIHandler( const QString& keyword, ObjectGUIHandler* );
        
protected:
        ObjectHandler( const QString& kw );

        ObjectGUIHandler* guiHandler_;        

        
private:
        virtual DrawObject* parseObject( QTextStream& is, const Figure* fig ) = 0;

        ObjectHandler( const ObjectHandler& );

        static Initialiser::AutoHash<ObjectHandler> ohHash_;
};


//! Templated concrete class based on ObjectHandler
/*! The constructor registers the DrawObject described by the template
 *  parameter ObjectType to the ohHash_ by calling
 *  ObjectHandler::ObjectHandler().
 *
 *  The function parseObject is to be specialised for any ObjectTypes
 *  that need to read specific data.
 */
template<typename ObjectType>
class TObjectHandler : public ObjectHandler
{
public:
        TObjectHandler<ObjectType>()
                : ObjectHandler( ObjectType::cuteMetaObject().keyword() )
        {}

        virtual DrawObject* parseObject( QTextStream&, const Figure* ) 
        {
                return new ObjectType();
        }
        
};



#endif
