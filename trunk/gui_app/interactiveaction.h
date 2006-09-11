 
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

#ifndef interactiveaction_h
#define interactiveaction_h


#include <QAction>

#include "fig.h"
#include "actionstatus.h"

class DrawObject;
class Controler;

class Selection;
class ViewBase;

class QKeyEvent;



//! Abstract baseclass for all unseractions
/** CuteFig uses classes that are derived InteractiveAction for every
 *  user action, that needs user interaction. The InteractiveActions
 *  announce themselves to the Controler, and the Controler calls them
 *  back as soon as the userinteraction takes place. All operations
 *  are carried out on the Selection& selection_ which is a reference
 *  to the Selection of the Controler.
 *
 *  As InteractiveAction is derived from QAction, the
 *  InteractiveActions only need to be registered in an
 *  ActionGroup. This allows to easily write extensions. Only a new
 *  class derived from InteractiveAction needs to be implemented. This
 *  only needs to be added to an ActionCollection.
 *
 *  An action is active when the next mouse click is relevant for that
 *  action. This is normally stored in
 *  Controler::actionIsActive_. Some actions e.g. all the ones that
 *  inherit from CreateAction are active right from the moment they
 *  are waken up.
 *  
 *  There are the following abstract methods in InteractiveAction that
 *  should be reimplemented:
 *    - click() to handle the user click
 *    - handleSelection() to decide what to do with the selection right 
 *      after the action got activated
 *    - move() to handle a mousemove
 *    - keyStroke() to handle key strokes
 *    - wouldHandle() and (not as often) wouldHandleSelection() to state 
 *      whether the action would handle the DrawObject or Selection at 
 *      that point.
 *    - isActive() to state wether the InteractiveAction is active right 
 *      from the bginning.
 *    - reset() which is called by wakeup(). Supposed to do cleanups.
 *
 *  Further the action text, i.e. the to text to appear in a
 *  menubar, should be set by QAction::setText(). Optionally a
 *  keyboard shortcut, a toolbar icon can be set by the respective
 *  methods of QAction. A QCursor to appear when the action is aware
 *  can be set by setting the protected member cursor_.
 *  
 *  The mechanism is the following: 
 *
 *  The action is called by the slot wakeup() which has been connected
 *  to the signal QAction::triggered() in the constructor if
 *  InteractiveAction. First the action announces itself at the
 *  Controler. Then the virtual function handleSelection is called,
 *  for actions, that want to do something without user
 *  interaction. If a user click comes, the action is asked whether it
 *  will handle the DrawObject (that also can be a nullpointer) by
 *  wouldHandle().  If yes click() is called that gets the DrawObject
 *  and the point clicked at as parameter.
 *
 *  At any time the action can call Controler::execAction() and pass
 *  the Controler an instance of a Command that the controler will
 *  execute.
 */

class InteractiveAction : public QAction
{
        Q_OBJECT
public:
        InteractiveAction( Controler* parent );
        ~InteractiveAction() {}

        virtual void click( const QPoint& p, Fig::PointFlags f, const QMatrix& m ) = 0;
        virtual void move( const QPoint&, const QMatrix& ) {}
        virtual bool keyStroke( const QKeyEvent* ) { return false; }
        virtual bool inputMethodEvent( const QInputMethodEvent* ) { return false; }
        void modifierChange( Qt::KeyboardModifiers mods );

        virtual bool isActive() const = 0;
        virtual bool wouldHandle( DrawObject* o, const QPoint& p=QPoint(), const QMatrix* m=0 )=0;
        virtual bool wouldHandleSelection( const QPoint& p = QPoint(), const QMatrix* m = 0 );
        virtual bool wantsSnap( const QPoint&, const QMatrix& ) { return true; }
        virtual bool acceptsKeyStrokes() { return false; }

        virtual void drawMetaData( QPainter*, const ViewBase* ) const {}
        
        const QCursor& cursor() const { return cursor_; }

        virtual const QString commandName() const = 0;
        virtual const QString completeName() const;

        void wakeupAsToolAction();
        
        //! resets the action. (called by wakeup())
        /*! supposed to do cleanups */
        virtual void reset() {}

        const ActionStatus& status() const { return status_; }

signals:
        void statusChanged( const ActionStatus& st );

protected:
        Controler* controler_; //!< the Controler instance
        Selection& selection_; //!< the selection_

        QCursor cursor_;       //!< the cursor to appear during the action

        ActionStatus status_;

        virtual void handleSelection() {}
        void setInitialStatus();
        virtual void setInitialStatus_private();
        virtual QString defaultFirstClickHelp() const;
        virtual void modifierChange_private( Qt::KeyboardModifiers ) {}

        virtual bool event( QEvent* e );

        void emitStatus();
        
protected slots:
        void selectionChanged();
        virtual void wakeup(); 
};







        

#endif
