 
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
#include "allactions.h"
#include "allodialogs.h"

#include "controler.h"

CreateAction* EllipseHandler::createAction( Controler* c, QActionGroup* g )
{
        return new EllipseCAction( c, g );
}

ObjectDialog* EllipseHandler::editDialog( DrawObject* o, EditdialogAction* a, 
                                          QWidget* parent )
{
        return new EllipseDialog( o, a, parent );
}

CreateAction* PolylineHandler::createAction( Controler* c, QActionGroup* g )
{
        return new PolylineCAction( c, g );
}

ObjectDialog* PolylineHandler::editDialog( DrawObject* o, EditdialogAction* a, 
                                           QWidget* parent )
{
        return new PolylineDialog( o, a, parent );
}

CreateAction* PolygonHandler::createAction( Controler* c, QActionGroup* g )
{
        return new PolygonCAction( c, g );
}

ObjectDialog* PolygonHandler::editDialog( DrawObject* o, EditdialogAction* a, 
                                          QWidget* parent )
{
        return new PolygonDialog( o, a, parent );
}

CreateAction* TextHandler::createAction( Controler* c, QActionGroup* g )
{
        return new TextAction( c, g );
}

ObjectDialog* TextHandler::editDialog( DrawObject* o, EditdialogAction* a, QWidget* parent )
{
        return 0;
}
