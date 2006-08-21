 
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

#include "length.h"

#include <QDebug>


Length::Length( double value )
        : value_( value ),
          baseLengthRU_( 0 )
{
}

Length::Length( double value, const ResourceKey& key )
        : value_( value ),
          baseLengthRU_( new ResourceUser<Length>( key ) )
{
}

Length::Length( const Length& other )
        : value_( other.value_ ),
          baseLengthRU_( other.baseLengthRU_ ? other.baseLengthRU_->clone() : 0 )
{
}

Length::~Length()
{
        delete baseLengthRU_;
}

Length& Length::operator=( const Length& other )
{
        if ( this == &other )
                return *this;
        
        value_ = other.value_;
        delete baseLengthRU_;
        baseLengthRU_ = other.baseLengthRU_ ? other.baseLengthRU_->clone() : 0;
        
        return *this;
}

Length::operator double() const
{
        return baseLengthRU_
                ? value_ * static_cast<ResourceUser<Length>*>( baseLengthRU_ )->data()
                : value_;
}


template<>
void ResLib<Length>::init()
{
        insertBuiltIn( "inch", Length( 72. ) );
        insertBuiltIn( "mm", Length( 2.83 ) );
        //insertBuiltIn( "cm", Length( 10.0, ResourceKey::builtIn("mm") ) );
}

template<>
ResourceKey ResLib<Length>::defaultKey()
{
        return ResourceKey::builtIn("inch");
}

namespace Res
{
        template<> const QString resourceName<Length>() { return "length"; }
}

