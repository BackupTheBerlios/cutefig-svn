 
/*****************************************************************************
**
**  $Id$
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

#include "actionpushbutton.h"

ActionPushButton::ActionPushButton( QAction* action, QWidget* parent )
        : QPushButton( parent ),
          action_( action )
{
        actionChanged();
        connect( action_, SIGNAL( changed() ), this, SLOT( actionChanged() ) );
        connect( this, SIGNAL( clicked() ), action_, SLOT( trigger() ) );
}

void ActionPushButton::actionChanged()
{
        setText( action_->text() );
        setIcon( action_->icon() );
        setEnabled( action_->isEnabled() );
        setToolTip( action_->toolTip() );
//        setShortcut( action_->shortcut() );
}
