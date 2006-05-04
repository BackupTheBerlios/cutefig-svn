 
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

#include "resourcecombobox.h"

#include <QDebug>

AbstractResourceComboBox::AbstractResourceComboBox( const ResourceKeyList keys, QWidget* parent )
        : QComboBox( parent ),
          keys_( keys )
{
        connect( this, SIGNAL( activated(int) ), this, SLOT( mapActivate(int) ) );
        connect( this, SIGNAL( highlighted(int) ), this, SLOT( mapHighlight(int) ) );
}

void AbstractResourceComboBox::mapActivate( int i )
{
        emit activated( keys_[i] );
}

void AbstractResourceComboBox::mapHighlight( int i )
{
        emit highlighted( keys_[i] );
}

void AbstractResourceComboBox::setCurrentKey( const ResourceKey& key )
{
        QComboBox::setCurrentIndex( keys_.indexOf( key ) );
}

ResourceKey AbstractResourceComboBox::currentKey() const
{
        return keys_[currentIndex()];
}

