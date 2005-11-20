 
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


#include "textaction.h"
#include "textobject.h"
#include "addcommand.h"

#include <QKeyEvent>
#include <QApplication>

#include <QDebug>

void TextAction::click( const QPoint& p, Fig::PointFlag f, const QMatrix* m )
{
        textObject_->pointSet( m->map( QPointF( p ) ), f );
        pointIsSet_ = true;
        textObject_->setCursorPos( 0 );
        cursorTimer_ = startTimer( QApplication::cursorFlashTime()/2 );
}


bool TextAction::keyStroke( const QKeyEvent* ke )
{
        if ( !pointIsSet_ )
                return false;
        
        if ( ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Enter ) {
                killTimer( cursorTimer_ );
                textObject_->hideCursor();
                controler_->execAction( new AddCommand( selection_ ) );
                selection_.updateBackups();
                pointIsSet_ = false;
                return true;
        }

        bool handled = true;
        
        switch ( ke->key() ) {
            case Qt::Key_Backspace:
                    textObject_->removeCharBackward(); break;
            case Qt::Key_Delete:
                    textObject_->removeCharForward(); break;
            case Qt::Key_Left:
                    textObject_->decrementCursorPos(); break;
            case Qt::Key_Right:
                    textObject_->incrementCursorPos(); break;
            case Qt::Key_Home:
                    textObject_->setCursorPos( 0 ); break;
            case Qt::Key_End:
                    textObject_->moveCursorToEnd(); break;
            default:
                    handled = false;
        }

        if ( handled )
                return true;
        
        QString t = ke->text();
        if ( !t.isEmpty() ) {
                textObject_->insertByCursor( t );
                return true;
        }

        return false;
}

void TextAction::reset()
{
        pointIsSet_ = false;
        textObject_ = 0;
}

DrawObject* TextAction::createObject()
{
        textObject_ = new TextObject( controler_->figure() );
        return textObject_;
}

bool TextAction::event( QEvent* e )
{
        if ( e->type() == QEvent::Timer ) {
                if ( textObject_ ) {
                        textObject_->toggleCursor();
                        controler_->updateViews();
                }
                return true;
        }
        
        return QAction::event(e);
}
