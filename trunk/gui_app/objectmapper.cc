 
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

#include "objectmapper.h"
#include "objecthandler.h"
#include "drawobject.h"

#include <QWidget>
#include <QDebug>

ObjectMapper::~ObjectMapper()
{
        foreach ( ObjectHandler* oh, objectMap_ )
                delete oh;
}

ObjectDialog* ObjectMapper::editDialog( DrawObject* o, EditdialogAction* a,
                                        QWidget* w )
{    
        if ( !o )
                return 0;
        
        ObjectHandler* h = lookup( o->objectKeyWord() );
        if ( !h )
                return 0;

        return h->editDialog( o, a, w );
}

bool ObjectMapper::registerItem( ObjectHandler* h )
{
        const QString keyword = h->keyword();
        qDebug() << "registering" << keyword;
        
        if ( objectMap_.contains( keyword ) )
                return false;

        objectMap_.insert(keyword, h );
        handlers_ << h;
        return true;
}

ObjectHandler* ObjectMapper::lookup( const QString& keyword )
{
     ObjectHandler* h = objectMap_.value( keyword );
     if ( !h ) 
                qDebug() << "DrawObject " << keyword 
                         << " not registered in ObjectMapper";
               
     return h;
}
