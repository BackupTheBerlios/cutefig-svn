 
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

#ifndef editdialogaction_h
#define editdialogaction_h

#include "interactiveaction.h"

class ObjectDialog;

class EditdialogAction : public InteractiveAction
{
        Q_OBJECT
public:
        friend class ObjectDialog;

        EditdialogAction( Controler* parent )
                : InteractiveAction( parent ) {
                setText("Edit Object");
                setIcon( QIcon(":images/edit.png") );
                setWhatsThis( tr("Edit the object's properties") );
                setShortcut( Qt::CTRL+Qt::Key_E );
                cursor_ = Qt::PointingHandCursor;
        }
        ~EditdialogAction() {}

        virtual void click( const QPoint& p, Fig::PointFlag f, const QMatrix* m );
        virtual void handleSelection();
        virtual bool wouldHandle( DrawObject*, const QPoint& = QPoint(), const QMatrix* =0 ) { 
                return true; 
        }
        
        virtual bool wouldHandleSelection( const QPoint& p = QPoint(), const QMatrix* = 0 );

        virtual const QString commandName() const { return "Edit"; }

        bool isActive() const { return false; } 

private slots:
        void wObjectHasChanged();

private:
        void editObject();

        bool wObjectChanged_;

        DrawObject* restoreWObject();
};

#endif
