 
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

#include "texttagaction.h"
#include "textaction.h"
#include "textobject.h"

#include <QDebug>

TextTagAction::TextTagAction( Controler* parent, QActionGroup* g )
        : TextPropAction( parent, g )
{
        setCheckable( true );
//        disconnect( this, SLOT( wakeup() ) );
//        connect( this, SIGNAL( changed() ), this, SLOT( wakeup() ) );
}

bool TextTagAction::wouldHandle( DrawObject*, const QPoint&, const QMatrix* )
{
        qDebug() << "TextTagAction::wouldHandle()";
        
        if ( ! textAction_->isEditing() )
                return false;

        qDebug() << "still here";
        
        setChecked( correctState( textAction_->textObject() ) );
        return true;
}

void TextTagAction::handleObject( TextObject* to )
{
        setChecked( toggleState( to ) );
}

void TextTagAction::wakeup()
{
//        if ( textAction_->isEditing() )
        qDebug() << "wakeup()";
        handleObject( textAction_->textObject() );
        connect( this, SIGNAL( changed() ), this, SLOT( wakeup() ) );
        qDebug() << isEnabled();
}

// void TextTagAction::setStateCorrectly()
// {
//         if ( !textAction_->isEditing() )
//                 return;
        
//         setChecked( correctState( textAction_->textObject() ) );
// }

// bool TextTagAction::shouldBeChecked()
// {
//         if ( textAction_->isEditing() )
//                 return false;
        
// }

