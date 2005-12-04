 
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

#ifndef dobjectfactory_h
#define dobjectfactory_h

#include "typedefs.h"

#include <QHash>
#include <QString>

#include <istream>
class DrawObjectIO;
class DrawObject;
class Figure;

//! Constructs a DrawObject described by a keyword
/*! 
 */
class DrawObjectFactory
{
public:
        static DrawObject* getDrawObject( const QString& keyword, std::istream& is, Figure* fig );

        virtual ~DrawObjectFactory() {}

protected:
        DrawObjectFactory( const QString& kw )
        {
                dFHash_[kw] = this;
        }

        virtual DrawObject* parseObject( std::istream& is, Figure* fig ) = 0;
        
private:
        DrawObjectFactory( const DrawObjectFactory& ) {}

        static QHash<QString, DrawObjectFactory*> dFHash_;
};

template<typename ObjectType>
class TDrawObjectFactory : public DrawObjectFactory
{
public:
        TDrawObjectFactory<ObjectType>()
                : DrawObjectFactory( DObjects::objectKeyWord<ObjectType>() )
        {}

        virtual DrawObject* parseObject( std::istream&, Figure* fig ) 
        {
                return new ObjectType( fig );
        }
};


#endif
