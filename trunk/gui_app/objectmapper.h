 
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

#ifndef objectmapper_h
#define objectmapper_h

#include <QHash>
#include <QString>

class CreateAction;
class EditdialogAction;
class ObjectDialog;
class ObjectHandler;
class CreateActions;
class DrawObject;
class QWidget;

/** \class ObjectMapper
 *  \brief Maps a QString to an ObjectHandler
 *
 *  Only one instance of ObjectMapper is possible. New DrawObjects can
 *  be registered using registerItem(). For convenience CreateActions
 *  is declared friend, because it just takes the objectMap_ and sets
 *  up the CreateAction subclasses.
 *
 *  For the lookup \link DrawObject::objectname()
 *  DrawObject::objectname() \endlink is used.
 */

class ObjectMapper
{
        ObjectMapper() : objectMap_() {}

public:
        friend class CreateActions;

        ~ObjectMapper();

        static ObjectMapper& instance() 
        {
                static ObjectMapper* inst = new ObjectMapper();
                return *inst;
        }

        ObjectDialog* editDialog( DrawObject* o, EditdialogAction* a, QWidget* w );

        bool registerItem( ObjectHandler* h );

private:
        QHash<QString, ObjectHandler*> objectMap_;
        ObjectHandler* lookup( const QString& name );
};

#endif
