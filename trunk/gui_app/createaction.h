 
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

#ifndef createaction_h
#define createaction_h 

#include "interactiveaction.h"
#include "allobjects.h"

/** \class CreateAction
 *  \brief A base class for create actions of DrawObjects
 *
 *  Each DrawObject that can be createt from skratch must come up with
 *  a subclass of CreateAction.
 */

class CreateAction : public InteractiveAction
{
        Q_OBJECT
protected:
        CreateAction( Controler* parent, QActionGroup* group = 0 ) 
                : InteractiveAction( parent, group ) {}
public:
        ~CreateAction() {}

        virtual void click( const QPoint& p, Fig::PointFlag f, const QMatrix* m );
        virtual void move( const QPoint& p, const QMatrix* m );

        virtual bool wouldHandle( DrawObject*, const QPoint& = QPoint(), const QMatrix* =0 ) { 
                return true; }

        virtual bool isActive() const { return true; }

        virtual const QCursor cursor( DrawObject* ) const { return Qt::PointingHandCursor; }
        
        virtual void handleSelection();

        virtual const QString commandName() const { return "create"; }


private:
        virtual DrawObject* createObject() = 0;
        DrawObject* cObject_;
};


class EllipseCAction : public CreateAction
{
        Q_OBJECT
public:
        EllipseCAction( Controler* parent, QActionGroup* group = 0 )
                : CreateAction( parent, group ) {
                setText( tr("Create &Ellipse") );
                setShortcut( Qt::Key_E );
                setIcon( QIcon(":images/ellipse.png") );
                cursor_ = Qt::UpArrowCursor;
        }

protected:
        virtual DrawObject* createObject()
        { return new Ellipse( controler_->figure() ); }
};

class PolylineCAction : public CreateAction
{
        Q_OBJECT
public:
        PolylineCAction( Controler* parent, QActionGroup* group = 0 )
                : CreateAction( parent, group ) {
                setText( tr("Create Poly&line") );
                setShortcut( Qt::Key_L );
                setIcon( QIcon(":images/polyline.png") );
                cursor_ = Qt::UpArrowCursor;
        }

protected:
        virtual DrawObject* createObject()
        { return new Polyline( controler_->figure() ); }
};

class PolygonCAction : public CreateAction
{
        Q_OBJECT
public:
        PolygonCAction( Controler* parent, QActionGroup* group = 0 )
                : CreateAction( parent, group ) {
                setText( tr("Create &Polygon") );
                setIcon( QIcon(":images/polygon.png") );
                setShortcut( Qt::Key_P );
                cursor_ = Qt::UpArrowCursor;
        }

protected:
        virtual DrawObject* createObject()
        { return new Polygon( controler_->figure() ); }
};

#endif
