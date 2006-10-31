 
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

#include "objecthandler.h"
#include "initialiser.h"


Initialiser::AutoHash<ObjectHandler> ObjectHandler::ohHash_;

ObjectHandler::ObjectHandler( const QString& kw )
        : guiHandler_( 0 )
{
        ohHash_[kw] = this;
}

/*! \param keyWord is the keyword of the DrawObject. \param is is an
 *  input stream out of which parseDrawObject() is to read the object
 *  specific data. \param fig is the figure to which the DrawObject
 *  will belong.
 */
DrawObject* ObjectHandler::getDrawObject( const QString& keyWord, QTextStream& is, Figure* fig )
{
        ObjectHandler* h = ohHash_[keyWord];

        DrawObject* o = 0;

        if ( h )
                o = h->parseObject( is, fig );
        
        return o;
}

void ObjectHandler::registerGUIHandler( const QString& keyword, ObjectGUIHandler* gh )
{
        ObjectHandler* h = ohHash_[keyword];

        if ( h )
                h->guiHandler_ = gh;
}

ObjectGUIHandler* ObjectHandler::guiHandler( const QString& keyWord )
{
        ObjectHandler* h = ohHash_[keyWord];

        return h ? h->guiHandler_ : 0;
}

