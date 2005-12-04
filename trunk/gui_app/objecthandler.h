 
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

class Controler;
class QActionGroup;
class DrawObject;
class QWidget;
class EditdialogAction;

#include "dobjectfactory.h"
#include "createaction.h"
#include "objectdialog.h"

#include <QString>


/** \class ObjectHandler
 *  \brief An abstract base class to handle DrawObjects
 *
 *  Only one instance of each ObjectHandler's subclasses is
 *  possible. An ObjectHandler provides two funcions:
 *     - createAction(), which returns the CreateAction that's appropriate
 *     - editDialog(), which returns the appropriate ObjectDialog
 *
 *  So each subclass of DrawObject, that wants to be created or
 *  modified by an ObjectDialog must come up with a subclass of
 *  ObjectHandler. This subclass has to be registered to the
 *  ObjectMapper by ObjectMapper::registerItem().
 */

class ObjectHandler
{
protected:
        ObjectHandler() {};

public:
        virtual ~ObjectHandler() {};

        virtual CreateAction* createAction( Controler* c, QActionGroup* g )=0;
        virtual ObjectDialog* editDialog( DrawObject* o, 
                                          EditdialogAction* a, 
                                          QWidget* parent )=0;
        virtual const QString keyword() const = 0;
};

template<typename ObjectType>
class TObjectHandler : public ObjectHandler
{
        TObjectHandler<ObjectType>() : ObjectHandler() {}
public:
        static TObjectHandler<ObjectType>* instance() {
                static TObjectHandler<ObjectType>* inst = new TObjectHandler<ObjectType>();
                return inst;
        }

        CreateAction* createAction( Controler* c, QActionGroup* g ) 
        {
                return new TCreateAction<ObjectType>( c, g );
        }
        
        ObjectDialog* editDialog( DrawObject* o, EditdialogAction* a, QWidget* parent )
        {
                return new ObjectDialog( o, a, parent );        
        }
        
        const QString keyword() const { return DObjects::objectKeyWord<ObjectType>(); }
};

// class EllipseHandler : public ObjectHandler
// {
//         EllipseHandler() : ObjectHandler() {}

// public:
//         static EllipseHandler* instance() {
//                 static EllipseHandler* inst = new EllipseHandler();
//                 return inst;
//         }

//         CreateAction* createAction( Controler* c, QActionGroup* g );
//         ObjectDialog* editDialog( DrawObject* o, 
//                                   EditdialogAction* a, QWidget* parent );
        
//         const QString name() const { return "ellipse"; };
// };

// class PolylineHandler : public ObjectHandler
// {
//         PolylineHandler() : ObjectHandler() {}

// public:
//         static PolylineHandler* instance() {
//                 static PolylineHandler* inst = new PolylineHandler();
//                 return inst;
//         }

//         CreateAction* createAction( Controler* c, QActionGroup* g );
//         ObjectDialog* editDialog( DrawObject* o, 
//                                   EditdialogAction* a, QWidget* parent );

//         const QString name() const { return "polyline"; }
// };

// class PolygonHandler : public ObjectHandler
// {
//         PolygonHandler() : ObjectHandler() {}
// public:
//         static PolygonHandler* instance() {
//                 static PolygonHandler* inst = new PolygonHandler();
//                 return inst;
//         }

//         CreateAction* createAction( Controler* c, QActionGroup* g );
//         ObjectDialog* editDialog( DrawObject* o, 
//                                   EditdialogAction* a, QWidget* parent );

//         const QString name() const { return "polygon"; }
// };


// class TextHandler : public ObjectHandler
// {
//         TextHandler() : ObjectHandler() {}
// public:
//         static TextHandler* instance() {
//                 static TextHandler* inst = new TextHandler();
//                 return inst;
//         }

//         CreateAction* createAction( Controler* c, QActionGroup* g );
//         ObjectDialog* editDialog( DrawObject* o, 
//                                   EditdialogAction* a, QWidget* parent );

//         const QString name() const { return "text"; }
// };

#endif
