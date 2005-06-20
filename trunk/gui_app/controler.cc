 
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

/** \class Controler
 *  
 *  The controler handles all the user interaction and manipulates the
 *  Figure. After it changed the Figure all the views of viewList_ get
 *  the ViewBase::updateFigure() command. The Controler keeps track of
 *  all user interaction by storing all the commands in the
 *  commandStack_. The commandStack_ is used by the undo() and redo()
 *  functions that are called by the coresponding actions.
 *
 */

#include "controler.h"
#include "allobjects.h"
#include "interactiveaction.h"
#include "actions.h"

#include <QEvent>
#include <QApplication>
#include <QDebug>

Controler::Controler( QMainWindow * parent )
        : QObject( parent ),
          mainWindow_( parent ),
          editAction_( 0 ),
          actionIsActive_( 0 ),
          commandStack_(),
          currentCommand_( commandStack_ ),
          selection_(),
          figureChanged_( false ),
          toolDepth_( 50 ),
          toolActionsGroup_( 0 )
{
}

void Controler::setFigure( Figure* f )
{
        figure_ = f;
        f->setControler( this );
        clearCommandList();
}

/** Resets the selection_. 
 */
void Controler::selectObject( DrawObject* o )
{
        if ( selection_.select( o ) )
                updateViews();
}


/** Asks the editAction_ whether it will handle the DrawObject o or
 * the current selection_ respectively, if the user clicks at p, and
 * passes a suitable QCursor back to the view to appear.
*/
const QCursor Controler::considerObject( DrawObject* o, const QPoint& p, const QMatrix* m ) const
{
        if ( editAction_ && editAction_->wouldHandle( o, p, m ) )
                return editAction_->cursor();
                                               
        if ( o )
                return QCursor( Qt::PointingHandCursor );
        else
                return QCursor( Qt::ArrowCursor );
}

const QCursor Controler::findToolAction( const QPoint& p, const QMatrix* m )
{
        if ( editAction_ ) { 
                if ( editAction_->wouldHandleSelection( p, m ) ) 
                        return editAction_->cursor();
        } else
                foreach ( InteractiveAction* a, activeToolActions_ )  
                        if ( a->wouldHandleSelection( p, m ) ) {
                                a->reset();
                                editAction_ = a;
                                return a->cursor();
                        }

        if ( !explicitEAction_ )
                editAction_ = 0;

        return Qt::ArrowCursor;
}

/** Usually called by InteractiveAction::getActive(). 
 */
void Controler::newAction( InteractiveAction* a )
{
        editAction_ = a;
        explicitEAction_ = true;
        actionIsActive_ = a->isActive();
        updateViews();
}

void Controler::cancelAction()
{
        if ( !editAction_ )
                return;

        editAction_ = 0;
        explicitEAction_ = false;
        actionIsActive_ = 0;
        selection_.restoreBackups();
        updateViews();
}

void Controler::execAction( Command* cmd )
{
        if ( cmd ) {
                cmd->setControler( this );
                cmd->setCommandName( editAction_->completeName() );
                cmd->execute();
                appendToCmdList( cmd );
        }
        editAction_ = 0;
        explicitEAction_ = false;

        updateViews();
}

/** Called to pass a mouse move to the editAction_. 
 */
void Controler::callActionMove( const QPoint& p, const QMatrix* m )
{
        if ( !(actionIsActive_ && editAction_ && !selection_.isEmpty() ) )
                return;

        editAction_->move( p, m );
        updateViewsImediately( true );
}

/** Called to pass a mouse click to the editAction_
 */
const QCursor Controler::callActionClick( const QPoint& p, Fig::PointFlag f, const QMatrix* m )
{
        editAction_->click( p, f, m );
        actionIsActive_ = editAction_;
        updateViews();
        
        if ( editAction_ )
                return editAction_->cursor();

        return Qt::ArrowCursor;
}

bool Controler::willAcceptAction( const QPoint& p, const QMatrix* m ) const
{
        return editAction_ && editAction_->wouldHandleSelection( p, m );
}

bool Controler::wouldAcceptAction( const QPoint& p, const QMatrix* m ) const
{
        return actionIsActive_ && willAcceptAction( p, m );
}


/** Returns all controlpoints of all DrawObjects that can be handled
 * by editingAction_. If there is already a wObject_ registered only
 * this one is considered handelable.
 */
const QPolygonF Controler::objectsPoints()
{
//         if ( !( editAction_ || !selection_.isEmpty() ) )
//                 return 0;

//         QPolygonF points;

//         if ( actionIsActive_ ) {
//                 if ( wObject_ )
//                         points = wObject_->points();
//         }  else {
//                 ObjectList::const_iterator it = figure_->objects().begin();
//                 for( ; it != figure_->objects().end(); ++it )
//                         if ( !editAction_ || editAction_->handles( *it ) )
//                                 points << (*it)->points();
//         }

//         return points;
        QPolygonF points;

        foreach ( DrawObject* o, selection_.objects() )
                points << o->points();

        return points;
}

void Controler::clearCommandList()
{
        currentCommand_.toFront();
        while ( currentCommand_.hasNext() )
                delete currentCommand_.next();
 
        commandStack_.clear();
        currentCommand_ = CmdStackIt( commandStack_ );
}

void Controler::undo()
{
        if ( ! currentCommand_.hasPrevious() ) {
                qDebug( "*** Oops *** Controler::undo() called without " 
                        "currentCommand_.hasPrevious() :-(" );
                return;
        }

        editActionsGroup_->undo( currentCommand_.peekPrevious()->commandString() );

        currentCommand_.previous()->unexecute();
        figureChanged_ = currentCommand_.hasPrevious();

        selectObject( 0 );
        updateViews();
}

void Controler::redo()
{
        if ( ! currentCommand_.hasNext() ) { 
                qDebug( "*** Oops *** Controler::redo() called without " 
                        "currentCommand_.hasNext() :-(" );
                return;
        }

        editActionsGroup_->redo( currentCommand_.peekNext()->commandString() );

        currentCommand_.next()->execute();
        figureChanged_ = true;

        selectObject( 0 );
        updateViews();
}


void Controler::undo( int steps )
{
        for ( int i = 0; i < steps; ++i )
                undo();
}

void Controler::redo( int steps )
{
        for ( int i = 0; i < steps; ++i )
                redo();
}

void Controler::appendToCmdList( Command* c )
{
        while ( currentCommand_.hasNext() ) {
                delete currentCommand_.next();
                currentCommand_.remove();
        }

        commandStack_.push_back( c );
        editActionsGroup_->addCommand( c->commandString() );
        currentCommand_.toBack();
        
        figureChanged_ = true;
}


void Controler::setToolProperties()
{
//         if ( !wObject_ ) {
//                 qDebug("Controler::setToolProperties shouldn't be called " 
//                        "without wObject_");
//                 return;
//         }

//         wObject_->setPen( toolPen_ );
//         wObject_->setFillColor( toolFillColor_ );
//         wObject_->setDepth( toolDepth_ );
}

void Controler::updateViewsImediately( bool tentative )
{
//        qDebug() << "updateViewsImediately" << tentative;
        foreach ( ViewBase* v, viewList_ )
                v->updateFigure( selection_, tentative );
}

void Controler::updateViews()
{
        qDebug() <<"updateViews";
        QApplication::postEvent( this, new QEvent( QEvent::UpdateRequest ) );
}

bool Controler::event( QEvent* e )
{
        if ( e->type() == QEvent::UpdateRequest ) {
                updateViewsImediately();
                return true;
        }
        return QObject::event( e );
}

void Controler::setToolActionsGroup( ToolActions* ta )
{
        if ( toolActionsGroup_ )
                return;

        toolActionsGroup_ = ta;
        activeToolActions_.append( ta->scale() );
        activeToolActions_.append( ta->move() );
}
