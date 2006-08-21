 
/*****************************************************************************
**
**  @version $Id: cutefig.cc 114 2006-08-17 18:31:18Z joh $
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

#include "paper.h"

#include <QDebug>

Paper::Paper()
        : size_( QSizeF( 8.5, 11 ) ),
          unit_( ResourceKey::builtIn("inch") )
{
}

Paper::Paper( QSizeF size, const ResourceKey& key )
        : size_( size ),
          unit_( key )
{
}

Paper::Paper( const Paper& other )
        : size_( other.size_ ),
          unit_( other.unit_ )
{
}

QSizeF Paper::size() const
{
        return size_ * unit_.data();
}

template<>
void ResLib<Paper>::init()
{
        insertBuiltIn( "Letter", Paper( QSizeF( 8.5, 11 ), ResourceKey::builtIn("inch") ) );
        insertBuiltIn( "ISO A4", Paper( QSizeF( 210, 297 ), ResourceKey::builtIn("mm") ) );
}

template<>
ResourceKey ResLib<Paper>::defaultKey()
{
        return ResourceKey::builtIn("Letter");
}


namespace Res
{
        template<> const QString resourceName<Paper>() { return "paper"; } 
}

