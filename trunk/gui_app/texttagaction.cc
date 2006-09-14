 
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

#include <QTextCharFormat>

#include <QDebug>

TextTagAction::TextTagAction( Controler* parent )
        : TextPropAction( parent )
{
        setCheckable( true );
}

bool TextTagAction::wouldHandle( DrawObject*, const QPoint&, const QMatrix* )
{
        if ( ! textAction_->isEditing() )
                return false;

        setChecked( correctState( textAction_->textObject() ) );
        return true;
}

void TextTagAction::wakeup()
{
        toggleState( textAction_->textObject() );
}

void TextTagAction::adaptToTextFormat()
{
        TextObject* to = textAction_->textObject();
        
        setChecked( to && correctState( to ) );
}




TextBoldAction::TextBoldAction( Controler* parent )
                : TextTagAction( parent )
{
        setText( tr("&Bold") );
        setShortcut( Qt::ALT+Qt::Key_B );
        setIcon( QIcon(":images/text_bold.png") );
}

bool TextBoldAction::correctState( const TextObject* to )
{
        return to->charFormat().font().bold();
}

void TextBoldAction::toggleState( TextObject* to )
{
        to->toggleBold();
}



TextItalicAction::TextItalicAction( Controler* parent )
                : TextTagAction( parent )
{
        setText( tr("&Italic") );
        setShortcut( Qt::ALT+Qt::Key_I );
        setIcon( QIcon(":images/text_italic.png") );
}

bool TextItalicAction::correctState( const TextObject* to )
{
        return to->charFormat().font().italic();
}

void TextItalicAction::toggleState( TextObject* to )
{
        to->toggleItalic();
}



TextSuperScriptAction::TextSuperScriptAction( Controler* parent )
                : TextTagAction( parent )
{
        setText( tr("Su&perscript") );
        setShortcut( Qt::ALT+Qt::Key_AsciiCircum );
        setIcon( QIcon(":images/text_super.png") );
}

bool TextSuperScriptAction::correctState( const TextObject* to )
{
        return to->charFormat().verticalAlignment() == QTextCharFormat::AlignSuperScript;
}

void TextSuperScriptAction::toggleState( TextObject* to )
{
        to->toggleSuperScript();
}



TextSubScriptAction::TextSubScriptAction( Controler* parent )
                : TextTagAction( parent )
{
        setText( tr("&Subscript") );
        setShortcut( Qt::ALT+Qt::Key_Underscore );
        setIcon( QIcon(":images/text_sub.png") );
}

bool TextSubScriptAction::correctState( const TextObject* to )
{
        return to->charFormat().verticalAlignment() == QTextCharFormat::AlignSubScript;
}

void TextSubScriptAction::toggleState( TextObject* to )
{
        to->toggleSubScript();
}

