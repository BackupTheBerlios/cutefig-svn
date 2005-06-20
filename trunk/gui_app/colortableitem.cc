 
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


#include "colortableitem.h"

#include <QPainter>

ColorTableItem::ColorTableItem( Q3Table* table, const QColor& c )
        : Q3TableItem( table, Q3TableItem::Never )
{
        setColor( c );
}

void ColorTableItem::paint( QPainter* p, const QColorGroup&, 
                            const QRect& cr, bool )
{
        p->fillRect( 0,0,cr.width(),cr.height() , QBrush( color_ ) );
}
