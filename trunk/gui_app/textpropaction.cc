 
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

#include "textpropaction.h"
#include "textobject.h"
#include "controler.h"
#include "textaction.h"
#include "selection.h"
#include "changecommand.h"

#include <QDebug>

TextPropAction::TextPropAction( Controler* parent )
        : InteractiveAction( parent  ),
          textAction_( controler_->textAction() )
{
}

void TextPropAction::click( const QPoint&, Fig::PointFlag, const QMatrix* )
{
        handleSelection();
}

void TextPropAction::handleSelection()
{        
        bool didSomething = false;
                
        foreach ( DrawObject* o, selection_.objects() ) {
                TextObject* to = qobject_cast<TextObject*>( o );
                if ( to ) {
                        didSomething = true;
                        handleObject( to );
                }
        }

        if ( didSomething ) {
                controler_->execAction( new ChangeCommand( selection_ ) );
                selection_.updateBackups();
        }
}

bool TextPropAction::wouldHandle( DrawObject* o, const QPoint&, const QMatrix* )
{
        if ( !o )
                return false;
        
        return o->inherits("TextObject");
}


void TextPropAction::wakeup()
{
        if ( textAction_->isEditing() ) 
                handleObject( textAction_->textObject() );
        else
                InteractiveAction::wakeup();
}



void TextHCenterAction::handleObject( TextObject* to )
{
        to->alignHCenter();
}

void TextLeftAction::handleObject( TextObject* to )
{
        to->alignLeft();
}

void TextRightAction::handleObject( TextObject* to )
{
        to->alignRight();
}

void TextVCenterAction::handleObject( TextObject* to )
{
        to->alignVCenter();
}

void TextTopAction::handleObject( TextObject* to )
{
        to->alignTop();
}

void TextBottomAction::handleObject( TextObject* to )
{
        to->alignBottom();
}
