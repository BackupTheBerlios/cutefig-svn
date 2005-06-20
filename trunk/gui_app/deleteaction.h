 
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

#ifndef deleteaction_h
#define deleteaction_h

#include "interactiveaction.h"

class DeleteAction : public InteractiveAction
{
        Q_OBJECT
public:
        DeleteAction( Controler* parent, QActionGroup* group = 0 )
                : InteractiveAction( parent, group ) {
                setText("&Delete");
                setIcon( QIcon(":images/editdelete.png") );
                setShortcut( Qt::Key_Delete );
                cursor_ = Qt::ForbiddenCursor;
        }
        ~DeleteAction() {}

        virtual void click( const QPoint& p,Fig::PointFlag f, const QMatrix* m );


        virtual bool wouldHandle( DrawObject* o, const QPoint& = QPoint(), const QMatrix* = 0) { 
                return o; 
        }

        virtual bool isActive() const { return false; }
        virtual void handleSelection();

        virtual const QString commandName() const { return "delete"; }

private:
        void deleteSelection();
};

#endif
