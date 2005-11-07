 
/*****************************************************************************
**
**  $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2005 Johannes Mueller, johmue@users.sourceforge.net
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

#include "mouseeventhandler.h"


void AbstractMouseEventHandler::mouseMoveEvent( QMouseEvent* e )
{
        if ( dragging_ )
                dispatcher_->drag( e );
        else
                dispatcher_->move( e );

}

void ClickMouseEventHandler::mouseReleaseEvent( QMouseEvent* e )
{
        if ( dragging_ ) 
                dragging_ = dispatcher_->finalClick( e );
        else 
                dragging_ = dispatcher_->initialClick( e );
}


void DragMouseEventHandler::mousePressEvent( QMouseEvent* e )
{
        dragging_ = dispatcher_->initialClick( e );
}


void DragMouseEventHandler::mouseReleaseEvent( QMouseEvent* e )
{
        if ( dragging_ )
                dispatcher_->finalClick( e );
        dragging_ = false;
}
