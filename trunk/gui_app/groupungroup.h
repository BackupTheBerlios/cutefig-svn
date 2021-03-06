 
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

#ifndef groupungroup_h
#define groupungroup_h

#include "interactiveaction.h"

class GroupAction : public InteractiveAction
{
        Q_OBJECT
public:
        GroupAction( Controler* parent ); 
        ~GroupAction() {}

        virtual void click( const QPoint&, Fig::PointFlags, const QMatrix& ) {}

        virtual void handleSelection();

        virtual bool isActive() const { return false; }
        virtual bool wouldHandle( DrawObject*, const QPoint& = QPoint(), const QMatrix* = 0 ) { 
                return false; 
        }
        
        virtual bool wouldHandleSelection( const QPoint& = QPoint(), const QMatrix* m = 0 );

        virtual const QString commandName() const { return "group"; }

};

class UngroupAction : public InteractiveAction
{
        Q_OBJECT
public:
        UngroupAction( Controler* parent );
        ~UngroupAction() {}

        virtual void click( const QPoint&, Fig::PointFlags, const QMatrix& ) {}

        virtual void handleSelection();

        virtual bool isActive() const { return false; }
        virtual bool wouldHandle( DrawObject* o, const QPoint& = QPoint(), const QMatrix* m=0 );
        virtual bool wouldHandleSelection( const QPoint& = QPoint(), const QMatrix* m=0 );

        virtual const QString commandName() const { return "ungroup"; }
};

#endif
