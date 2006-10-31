 
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

#include "objectguihandler.h"
#include "objecthandler.h"
#include "drawobject.h"
#include "actioncollection.h"
#include "guiinit.h"

#include <QDebug>

QStringList ObjectGUIHandler::keyWords_;

class ObjectGUIHandler::Initialiser
{
public:
        Initialiser()
        {
                ::Initialiser::createGUIHandlers();
        }
};

ObjectGUIHandler::ObjectGUIHandler( const QString& keyword )
{
        ObjectHandler::registerGUIHandler( keyword, this );
        keyWords_ << keyword;
}

void ObjectGUIHandler::setupCreateActions( Controler* c, ActionCollection* ac )
{
        static Initialiser i;
        
        foreach ( QString kw, keyWords_ ) {
                ObjectGUIHandler* gh = ObjectHandler::guiHandler( kw );
                if ( gh )
                        ac->addAction( gh->makeCreateAction( c ) );
                else
                        qDebug() << "+++ BUG +++ no ObjectGUIHandler for" << kw;
        } 
}

ObjectDialog* ObjectGUIHandler::editDialog( DrawObject* o, EditdialogAction* a, QWidget* parent )
{
        const QString kw = o->objectKeyword();
        ObjectGUIHandler* gh = ObjectHandler::guiHandler( kw );
        if ( !gh ) {
                qDebug() << "+++ BUG +++ no ObjectGUIHandler for" << kw << "using default one.";
                return new ObjectDialog( o,a, parent );
        }
        
        return gh->makeEditDialog( o,a, parent );
}




