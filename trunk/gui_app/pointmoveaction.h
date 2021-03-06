 
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

#ifndef pointmoveaction_h
#define pointmoveaction_h

#include "interactiveaction.h"
#include "changecommand.h"

//! Moves one controlpoint of the DrawObject.
class PointMoveAction : public InteractiveAction
{
        Q_OBJECT
public:
        PointMoveAction( Controler* parent );

        virtual void click( const QPoint& p, Fig::PointFlags f, const QMatrix& m );
        virtual void move( const QPoint& p, const QMatrix& m );
        virtual bool wouldHandle( DrawObject* o, const QPoint& p = QPoint(), const QMatrix* m=0 );
        virtual bool wouldHandleSelection( const QPoint& p = QPoint(), const QMatrix* m=0 );

        virtual bool isActive() const { return false; }
        virtual void reset();

        virtual const QString commandName() const { return "move point"; }

        virtual void drawMetaData( QPainter* p, const ViewBase* v ) const;
        
private:
	//! the index of the point to be moved
        int pointIndex_;

	//! the DrawObject to be handeled
        DrawObject* wObject_;
};

#endif
