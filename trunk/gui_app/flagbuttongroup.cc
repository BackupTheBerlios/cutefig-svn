 
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

#include "flagbuttongroup.h"

#include <QAbstractButton>

#include <QDebug>

FlagButtonGroup::FlagButtonGroup( QWidget* parent )
        : QButtonGroup( parent ),
        signalMapper_( this )
{
        setExclusive( true );
        connect( &signalMapper_, SIGNAL( mapped(int) ), this, SLOT( changeState(int) ) );
}

bool FlagButtonGroup::addButton( QAbstractButton* b, int id )
{
        if ( signalMapper_.mapping( id ) )
                return false;

        connect( b, SIGNAL( toggled(bool) ), &signalMapper_, SLOT( map() ) );
        signalMapper_.setMapping( b, id );

        return true;
}

void FlagButtonGroup::changeState( int id )
{
        state_ = id;
        emit stateChanged( id );
}

void FlagButtonGroup::setState( int state )
{
        QAbstractButton* b = (QAbstractButton*) signalMapper_.mapping( state );
        
        if ( !b )
                return;

        state_ = state;
        b->setChecked( true );
}
