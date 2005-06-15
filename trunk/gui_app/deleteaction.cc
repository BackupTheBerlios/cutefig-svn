 
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

#include "deleteaction.h"
#include "deletecommand.h"

#include <QDebug>

void DeleteAction::click( const QPoint&, Fig::PointFlag, const QMatrix* )
{
        deleteSelection();
}

void DeleteAction::handleSelection()
{ 
        deleteSelection();
}

void DeleteAction::deleteSelection()
{
        if ( !selection_.isEmpty() ) {
                controler_->execAction( new DeleteCommand( selection_ ) );
                selection_.clear();
        }
        controler_->updateViews();
}
