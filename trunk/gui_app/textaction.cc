 
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
#include "textpropaction.h"
#include "textobject.h"
#include "addcommand.h"
#include "changecommand.h"
#include "objectguihandler.h"
#include "actions.h"
#include "figure.h"

#include <QKeyEvent>
#include <QInputMethodEvent>
#include <QTextCursor>
#include <QApplication>

#include <QDebug>

TextAction::TextAction( Controler* parent )
        : CreateAction( parent ),
          textObject_( 0 )
{
        setText( tr("Create &Text") );
        setIcon( QIcon(":images/text.png") );
        setShortcut( Qt::Key_T );
        cursor_ = Qt::IBeamCursor;
        controler_->setTextAction( this );
}

void TextAction::click( const QPoint& p, Fig::PointFlag f, const QMatrix* m )
{
        if ( textObject_ ) {
                commitTextObject();
                return;
        }

        QPoint point = m->map( p );

        TextObject* to = textObjectUnderPoint( point );
        
        if ( to ) {
                selection_.select( to, Selection::Exclusive );
                controler_->updateViews();
                textObject_ = static_cast<TextObject*>( selection_.objects()[0] );
                textObject_->setCursorToPoint( point );
                textNew_ = false;
        } else {
                textObject_ = new TextObject( controler_->figure() );
                selection_.setObjectToBeCreated( textObject_ );
                textObject_->pointSet( point, f );
                textObject_->setCursorPos( 0 );
                textNew_ = true;
        }
        
        connect( textObject_, SIGNAL( charFormatChanged() ), this, SLOT( dispatchFormat() ) );
        dispatchFormat();
        controler_->textPropActions()->setAllEnabled( true );
        cursorTimer_ = startTimer( QApplication::cursorFlashTime()/2 );
}


bool TextAction::keyStroke( const QKeyEvent* ke )
{
        if ( !textObject_ )
                return false;
        
        if ( ke->key() == Qt::Key_Enter || ke->key() == Qt::Key_Return ) {
                if ( !textObject_->isEmpty() )
                        commitTextObject();
                return true;
        }
 
        if ( cursorMovement( ke ) )
                return true;

        QString t = ke->text();
        if ( !t.isEmpty() ) {
                textObject_->insertByCursor( t );
                return true;
        }

        return false;
}

bool TextAction::cursorMovement( const QKeyEvent* ke )
{
        QTextCursor::MoveMode mode = QTextCursor::MoveAnchor;
        if ( ke->modifiers() & Qt::ShiftModifier )
                mode = QTextCursor::KeepAnchor;
        
        switch ( ke->key() ) {
            case Qt::Key_Backspace:
                    textObject_->removeCharBackward(); break;
            case Qt::Key_Delete:
                    textObject_->removeCharForward(); break;
            case Qt::Key_Left:
                    textObject_->decrementCursorPos( mode ); break;
            case Qt::Key_Right:
                    textObject_->incrementCursorPos( mode ); break;
            case Qt::Key_Home:
                    textObject_->setCursorPos( 0, mode ); break;
            case Qt::Key_End:
                    textObject_->moveCursorToEnd( mode ); break;
            case Qt::Key_Escape:
                    if ( !textObject_->clearSelection() )
                            controler_->cancelAction();
                    break;
            default:
                    return false;
        }
        
        return true;
}

void TextAction::commitTextObject()
{
        killTimer( cursorTimer_ );
        textObject_->hideCursor();

        if ( textNew_ )
                controler_->execAction( new AddCommand( selection_ ) );
        else
                controler_->execAction( new ChangeCommand( selection_ ) );
        
        selection_.updateBackups();
        controler_->textPropActions()->setAllEnabled( false );
}

bool TextAction::inputMethodEvent( const QInputMethodEvent* e )
{
        if ( !textObject_ )
                return false;
        
        textObject_->insertByCursor( e->commitString() );
        return true;
}

void TextAction::reset()
{
        textObject_ = 0;
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

void TextAction::dispatchFormat()
{
        foreach ( QAction* a, controler_->textPropActions()->actions() )
                static_cast<TextPropAction*>( a )->adaptToTextFormat();
}

TextObject* TextAction::textObjectUnderPoint( const QPointF& p )
{
        DrawObject* o = controler_->figure()->findContainingObject( p );
        return qobject_cast<TextObject*>( o );
}

bool TextAction::wantsSnap( const QPoint& p, const QMatrix* m )
{
        return ! ( textObject_ || textObjectUnderPoint( m->map( p ) ) );
}

const QString TextAction::commandName() const
{
        return textNew_ ? tr("create") : tr("edit");
}



template<>
CreateAction* TObjectGUIHandler<TextObject>::makeCreateAction( Controler* c )
{
        return new TextAction( c );
}

