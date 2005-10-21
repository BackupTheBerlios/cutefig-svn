 
/*****************************************************************************
**
**  $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2004 Johannes Mueller, johmue@users.sourceforge.net
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

#include "strokebutton.h"
#include "strokedialog.h"
#include "strokeiconengines.h"
#include "strokelib.h"

#include <QDebug>

StrokeButton::StrokeButton( const Stroke& initial, const ResourceKeyList& keys, QWidget* parent )
        : QPushButton( parent ),
          keys_( keys )
{
        setStroke( initial );
        connect( this, SIGNAL( clicked() ), this, SLOT( changeStroke() ) );
}

void StrokeButton::setStroke( const Stroke& stroke )
{
        stroke_ = stroke;
        setIcon( QIcon( new StrokeIconEngine( stroke ) ) );
}

void StrokeButton::changeStroke()
{
        bool ok;
        ResourceKey newKey = StrokeDialog::getStroke( stroke_.key(), &ok, keys_ );

        if ( ok ) {
                setStroke( StrokeLib::instance()[newKey] );
                emit strokeChanged( stroke_.key() );
        }
}
