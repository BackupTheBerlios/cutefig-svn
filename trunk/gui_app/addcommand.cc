 
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


#include "addcommand.h"
#include "figure.h"

#include <QDebug>

AddCommand::AddCommand( const Selection& s )
        : Command( s )
{
        setCommandName( tr("Add") );
}

void AddCommand::execute()
{
        foreach ( DrawObject* o, objects_ ) 
                figure_->addDrawObject( o );
}

void AddCommand::unexecute()
{
        foreach ( DrawObject* o, objects_ )
                figure_->removeDrawObject( o );
}
