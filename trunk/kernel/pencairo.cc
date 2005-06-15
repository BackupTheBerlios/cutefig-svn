 
/*****************************************************************************
**
**  @version $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2005 Johannes Mueller, joh@users.berlios.net
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

#include "pen.h"
#include "reslib.h"

Pen::Pen() 
        : color_( Qt::black ),
          lineWidth_( 0.5 ),
          offset_( 0.0 ),
          capStyle_( FlatCap ),
          joinStyle_( MiterJoin ),
          miterLimit_( 1.0 )
{
        setDashes( "SolidLine" );
}

Pen::Pen( double pw, const QString& dashKey, Dashes* dsh, double dofs,
          CapStyle cs, JoinStyle js, double ml,
          const QColor& c ) 
        : color_( c ),
          lineWidth_( pw ),
          dashKey_( dashKey ),
          dashes_( dsh ),
          offset_( dofs ),
          capStyle_( cs ),
          joinStyle_( js ),
          miterLimit_( ml )
{
}

bool Pen::setDashes( const QString& key )
{
        DashesLib& dl = DashesLib::instance();
        Dashes* dsh = dl[key];
        if ( dsh ) {
                dashes_ = dsh;
                dashKey_ = key;
                return true;
        }
        return false;
}
