 
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

#include "polygondialog.h"
#include "editdialogaction.h"

class Polygon;

PolygonDialog::PolygonDialog( DrawObject *o, EditdialogAction* a,
                              QWidget * parent )
        : ObjectDialog( o, a, parent )
{
        setWindowTitle( tr("Polygon Properties") );
}


template<>
ObjectDialog* TObjectGUIHandler<Polygon>::makeEditDialog( DrawObject* o, EditdialogAction* a, 
                                                   QWidget* parent )
{
        return new PolygonDialog( o, a, parent );
}
