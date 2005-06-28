 
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

#include "groupungroup.h"
#include "compound.h"
#include "changecommand.h"

#include <QDebug>

GroupAction::GroupAction( Controler* parent, QActionGroup* group )
        : InteractiveAction( parent, group )        
{
        setText("&Group");
        setIcon( QIcon(":images/group.png") );
        setShortcut( Qt::CTRL+Qt::Key_G );
}

void GroupAction::handleSelection()
{
        Compound* cpd = new Compound( selection_.objects(), controler_->figure() );
        selection_.setObjectToBeCreated( cpd, false );
        controler_->execAction( new ChangeCommand( selection_ ) );
        selection_.updateBackups();
}

bool GroupAction::wouldHandleSelection( const QPoint&, const QMatrix* )
{
        return selection_.objects().count() > 1;
}


UngroupAction::UngroupAction( Controler* parent, QActionGroup* group )
        : InteractiveAction( parent, group )
{
        setText( tr("&Ungroup") );
        setIcon( QIcon(":images/ungroup.png") );
        setShortcut( Qt::CTRL+Qt::Key_U );
}

void UngroupAction::handleSelection()
{
        Compound* cpd = qobject_cast<Compound*>( selection_.objects()[0] );
        if ( !cpd ) {
                qDebug() << "UngroupAction::handleSelection, cast to Compound failed";
                return;
        }

        cpd->releaseChildren();
        selection_.setListToBeInserted( cpd->childObjects(), false );
        controler_->execAction( new ChangeCommand( selection_ ) );
        selection_.updateBackups();
}

bool UngroupAction::wouldHandle( DrawObject* o, const QPoint&, const QMatrix* )
{
        return o && o->inherits( "Compound" );
}

bool UngroupAction::wouldHandleSelection( const QPoint&, const QMatrix* )
{
        return selection_.objects().count() == 1 && wouldHandle( selection_.objects()[0] );
}
