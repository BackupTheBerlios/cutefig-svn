 
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

#include "drawobject.h"
#include "clipboard.h"
#include "cutnpaste.h"
#include "addcommand.h"
#include "deletecommand.h"

#include <QDebug>

CopyAction::CopyAction( Controler* parent, QActionGroup* group )
        : InteractiveAction( parent, group ),
          clipboard_( Clipboard::instance() ) 
{
        setText( tr("&Copy") );
        setIcon( QIcon(":images/editcopy.png") );
        setShortcut( Qt::CTRL+Qt::Key_C );
        cursor_ = Qt::PointingHandCursor;
}

void CopyAction::click( const QPoint&, Fig::PointFlag, const QMatrix* )
{
        handleSelection();
}

void CopyAction::handleSelection()
{
        clipboard_->setContents( selection_.objects() );
        controler_->execAction( 0 );
        selection_.renewBackups();
        controler_->updateViews();
}


PasteAction::PasteAction( Controler* parent, QActionGroup* group )
                : InteractiveAction( parent, group ),
                  clipboard_( Clipboard::instance() ) 
{
        setText( tr("&Paste") );
        setIcon( QIcon(":images/editpaste.png") );
        setShortcut( Qt::CTRL+Qt::Key_V );
        cursor_ = Qt::PointingHandCursor;
        setEnabled( false );

        disconnect( this, SLOT( selectionChanged() ) );
        connect ( clipboard_, SIGNAL( hasContents( bool ) ), this, SLOT( setEnabled( bool ) ) );
}

bool PasteAction::wouldHandleSelection( const QPoint&, const QMatrix* )
{
        return !clipboard_->contents().isEmpty();
}

void PasteAction::handleSelection()
{
        const ObjectList& l = clipboard_->contents();

        if ( l.isEmpty() )
                return;

        selection_.setListToBeInserted( l );
        controler_->execAction( new AddCommand( selection_ ) );
        selection_.updateBackups();
}


CutAction::CutAction( Controler* parent, QActionGroup* group )
        : InteractiveAction( parent, group ),
          clipboard_( Clipboard::instance() )
{
        setText("Cu&t");
        setIcon( QIcon(":images/editcut.png") );
        setShortcut( Qt::CTRL+Qt::Key_X );
        cursor_ = Qt::ForbiddenCursor;
}

void CutAction::click( const QPoint&, Fig::PointFlag, const QMatrix* )
{
        handleSelection();
}

void CutAction::handleSelection()
{
        clipboard_->setContents( selection_.objects() );
        controler_->execAction( new DeleteCommand( selection_ ) );
        selection_.clear( false );
//        controler_->updateViews();
}
