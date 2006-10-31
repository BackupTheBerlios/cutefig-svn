 
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

#ifndef objectguihandler_h
#define objectguihandler_h

#include "typedefs.h"
#include "createaction.h"
#include "objectdialog.h"

#include <QStringList>

class ObjectDialog;
class EditdialogAction;
class ActionCollection;

class ObjectGUIHandler
{
public:
        static void setupCreateActions( Controler* c, ActionCollection* cltn );
        static ObjectDialog* editDialog( DrawObject* o, EditdialogAction* a, QWidget* parent  );
        
protected:
        ObjectGUIHandler( const QString& keyword );
        virtual ~ObjectGUIHandler() {}

        virtual CreateAction* makeCreateAction( Controler* c ) = 0;
        virtual ObjectDialog* makeEditDialog( DrawObject* o, EditdialogAction* a, QWidget* p ) =0;

private:
        static QStringList keyWords_;

        class Initialiser;
};


template<typename ObjectType>
class TObjectGUIHandler : public ObjectGUIHandler
{
public:
        TObjectGUIHandler<ObjectType>()
                : ObjectGUIHandler( ObjectType::cuteMetaObject().keyword() )
        {}

        CreateAction* makeCreateAction( Controler* c );
        ObjectDialog* makeEditDialog( DrawObject* o, EditdialogAction* a, QWidget* p );
};

template<typename ObjectType>
CreateAction* TObjectGUIHandler<ObjectType>::makeCreateAction( Controler* c )
{
        return new TCreateAction<ObjectType>( c );
}

template<typename ObjectType>
ObjectDialog* TObjectGUIHandler<ObjectType>::makeEditDialog( DrawObject* o, EditdialogAction* a,
                                                             QWidget* parent )
{
        return new ObjectDialog( o,a, parent );
}

        


#endif
