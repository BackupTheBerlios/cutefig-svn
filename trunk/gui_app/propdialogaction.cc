 
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

#include "propdialogaction.h"
#include "propdialog.h"
#include "figure.h"


class PropCommand : public Command
{
public:
        PropCommand( const Figure::MetaData& oldMD, const Figure::MetaData& newMD );

        virtual void execute();
        virtual void unexecute();
        
private:
        Figure::MetaData oldMD_, newMD_;
};



void PropDialogAction::handleSelection()
{
        QWidget* p = controler_->mainWindow();
        Figure* f = controler_->figure();
        
        PropDialog dlg( f, p );
        
        Figure::MetaData md( f->metaData() );

        connect( &dlg, SIGNAL( changeHappened() ), controler_, SLOT( updateFigureMetaData() ) );

        if ( dlg.exec() == QDialog::Accepted && dlg.editeeChanged() )
                controler_->execAction( new PropCommand( md, f->metaData() ) );
}



PropCommand::PropCommand( const Figure::MetaData& oldMD, const Figure::MetaData& newMD )
        : Command(),
          oldMD_( oldMD ),
          newMD_( newMD )
{
        setCommandName( tr("Edit figure properties") );
}

void PropCommand::execute()
{
        controler_->figure()->setMetaData( newMD_ );
        controler_->updateFigureMetaData();
}

void PropCommand::unexecute()
{
        controler_->figure()->setMetaData( oldMD_ );
        controler_->updateFigureMetaData();
}
