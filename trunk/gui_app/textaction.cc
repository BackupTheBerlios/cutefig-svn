 
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
        cursorPos_ = 0;
        textObject_->setCursorPos( 0 );
        cursorTimer_ = startTimer( QApplication::cursorFlashTime()/2 );
        textObject_->update();
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

        QString& text = textObject_->text();

        bool handled = true;
        
        switch ( ke->key() ) {
            case Qt::Key_Backspace:
                    if ( cursorPos_ )
                            text.remove( --cursorPos_, 1 );
                    break;
            case Qt::Key_Delete:
                    if ( cursorPos_ < text.length() ) 
                            text.remove( cursorPos_, 1 );
                    break;
            case Qt::Key_Left:
                    if ( cursorPos_ )
                            --cursorPos_;
                    break;
            case Qt::Key_Right:
                    if ( cursorPos_ < text.length() )
                            ++cursorPos_;
                    break;
            case Qt::Key_Home:
                    cursorPos_ = 0;
                    break;
            case Qt::Key_End:
                    cursorPos_ = text.length();
            default:
                    handled = false;
        }

        if ( handled ) {
                textObject_->setCursorPos( cursorPos_ );
                textObject_->update();
                return true;
        }
        
        QString t = ke->text();
        if ( !t.isEmpty() ) {
                text.insert( cursorPos_++, ke->text() );
                textObject_->setCursorPos( cursorPos_ );
                textObject_->update();
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

// void TextAction::timerEvent( QTimerEvent* )
// {
//         qDebug() << "timer";
// }

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
