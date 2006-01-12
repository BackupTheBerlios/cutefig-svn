 
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

#include <istream>
class DrawObjectIO;
class DrawObject;
class Figure;
class ObjectGUIHandler;

namespace Initialiser
{
        template<typename T> class AutoHash;
}


//! Constructs a DrawObject described by a keyword
/*! 
 */
class ObjectHandler
{
public:
        static DrawObject* getDrawObject( const QString& keyword, std::istream& is, Figure* fig );
        static ObjectGUIHandler* guiHandler( const QString& keyWord );
        virtual ~ObjectHandler() {}

        static void registerGUIHandler( const QString& keyword, ObjectGUIHandler* );
        
protected:
        ObjectHandler( const QString& kw );

        ObjectGUIHandler* guiHandler_;        

        virtual DrawObject* parseObject( std::istream& is, Figure* fig ) = 0;
        
private:
        ObjectHandler( const ObjectHandler& ) {}

        static Initialiser::AutoHash<ObjectHandler> ohHash_;
};

template<typename ObjectType>
class TObjectHandler : public ObjectHandler
{
public:
        TObjectHandler<ObjectType>()
                : ObjectHandler( ObjectType::cuteMetaObject().keyWord() )
        {}

        virtual DrawObject* parseObject( std::istream&, Figure* fig ) 
        {
                return new ObjectType( fig );
        }
        
};



#endif
