 
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


#ifndef relativeaction_h
#define relativeaction_h

#include "interactiveaction.h"
#include "drawobject.h"

//! Abstract base class to use mouse coordinates relative to the first mouse click.
class RelativeAction : public InteractiveAction
{
        Q_OBJECT
public:
        RelativeAction( Controler* parent )
                : InteractiveAction( parent ), 
                  finished_( true ),
                  last_() {}

        ~RelativeAction() {}
        
        virtual void click( const QPoint& p, Fig::PointFlags f, const QMatrix& m );
        virtual void move( const QPoint& p, const QMatrix& m );
        virtual bool wouldHandle( DrawObject* o, const QPoint& p = QPoint(), const QMatrix* m=0 );
        virtual bool isActive() const { return false; }
        virtual void reset();

protected:
        bool finished_;

        QPointF last_; /*!< absolute point at the last calculation */

private:
        virtual void relativeClick( const QPointF&, Fig::PointFlags f );
        virtual void relativeMove( const QPointF& );

        virtual void relativeClick( DrawObject* o, const QPointF&, Fig::PointFlags f ) = 0;
        virtual void relativeMove( DrawObject* o, const QPointF& ) = 0;
};


//! Quite simple action class that moves a DrawObject.
class MoveAction : public RelativeAction
{
        Q_OBJECT
public:
        MoveAction( Controler* parent )
                : RelativeAction( parent ) {
                setText( tr("&Move object") );
                setShortcut( Qt::ALT+Qt::Key_M );
                cursor_ = Qt::CrossCursor;
        }

        virtual const QString commandName() const { return "move"; }
        
private:
        virtual void relativeClick( DrawObject* o, const QPointF& p, Fig::PointFlags );
        virtual void relativeMove( DrawObject* o, const QPointF& p );
};


#endif
