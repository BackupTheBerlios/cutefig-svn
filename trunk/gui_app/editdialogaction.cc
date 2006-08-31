 
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


#include "editdialogaction.h"
#include "objectguihandler.h"
#include "objectdialog.h"
#include "changecommand.h"
#include "drawobject.h"
#include "controler.h"

#include <QWidget>

void EditdialogAction::click( const QPoint&, Fig::PointFlags, const QMatrix* )
{
        editObject();
}

void EditdialogAction::handleSelection()
{
        editObject();
}

void EditdialogAction::editObject()
{
        QWidget* p = controler_->mainWindow();
        ObjectDialog* dlg = ObjectGUIHandler::editDialog( selection_.objects()[0], this, p );
        connect( dlg, SIGNAL( changeHappened() ), controler_, SLOT( updateViews() ) );
        
        if ( dlg->exec() == QDialog::Rejected || !dlg->editeeChanged() ) 
                controler_->cancelAction();
        else {
                controler_->execAction( new ChangeCommand( selection_ ) );
                selection_.updateBackups();
        }

        delete dlg;
}

DrawObject* EditdialogAction::restoreWObject()
{
        selection_.restoreBackups();        
//        controler_->updateViews();
        return selection_.objects()[0];
}

// void EditdialogAction::wObjectHasChanged()
// {
//         wObjectChanged_ = true;
//         controler_->updateViews();
// }

bool EditdialogAction::wouldHandleSelection( const QPoint&, const QMatrix* )
{
        return ( selection_.objects().count() == 1 );
}
