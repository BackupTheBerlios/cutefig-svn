 
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

#ifndef mouseeventhandler_h
#define mouseeventhandler_h

class QMouseEvent;

class AbstractMouseEventDispatcher
{
public:
        virtual bool initialClick( QMouseEvent* e ) = 0;
        virtual bool finalClick( QMouseEvent* e ) = 0;
        virtual void drag( QMouseEvent* e ) = 0;
        virtual void move( QMouseEvent* e ) = 0;
};

class AbstractMouseEventHandler
{
public:
        AbstractMouseEventHandler( AbstractMouseEventDispatcher* dp )
                : dispatcher_( dp ),
                  dragging_( false )
        {}

        virtual void mousePressEvent( QMouseEvent* e ) = 0;
        virtual void mouseMoveEvent( QMouseEvent* e );
        virtual void mouseReleaseEvent( QMouseEvent* e ) = 0;

protected:
        AbstractMouseEventDispatcher* dispatcher_;
        bool dragging_;
};

class ClickMouseEventHandler : public AbstractMouseEventHandler
{
public:
        ClickMouseEventHandler( AbstractMouseEventDispatcher* dp )
                : AbstractMouseEventHandler( dp )
        {}
        
        virtual void mousePressEvent( QMouseEvent* ) {}
        virtual void mouseReleaseEvent( QMouseEvent* e );
};

class DragMouseEventHandler : public AbstractMouseEventHandler
{
public:
        DragMouseEventHandler( AbstractMouseEventDispatcher* dp )
                : AbstractMouseEventHandler( dp )
        {}
        
        virtual void mousePressEvent( QMouseEvent* e );
        virtual void mouseReleaseEvent( QMouseEvent* e );
};


#endif
