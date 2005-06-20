 
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

/** \class InteractiveAction
 *
 *  CuteFig uses classes that are derived InteractiveAction for every
 *  user action, that needs mouse interaction. The InteractiveActions
 *  register announce themselves to the Controler, and the Controler
 *  calls them back as soon as the userinteraction takes place. All
 *  operations are carried out on the Selection& selection_ which is a
 *  reference to the Selection of the Controler.
 *
 *  As InteractiveAction is derived from QAction, the
 *  InteractiveActions only need to be registered in an
 *  ActionGroup. This allows to easily write extensions. Only a new
 *  class derived from InteractiveAction needs to be implemented. This
 *  only needs to be added to an QActionGroup, which can also happen
 *  by passing a QActionGroup as a parameter to the constructor.
 *
 *  There are the following abstract methods in InteractiveAction that
 *  should be reimplemented:
 *    - click() to handle the user click
 *    - handleSelection() to decide what to do with the selection right 
 *      after the action got activated
 *    - move() to handle a mousemove
 *    - wouldHandle() and (not as often) wouldHandleSelection() to state 
 *      whether the action would handle the DrawObject or Selection at 
 *      that point.
 *    - isActive() to state wether the InteractiveAction is active right 
 *      from the bginning.
 *
 *  Further the action text, i.e. the to text to appear in a menubar,
 *  should be set by QAction::setText(). Optionally a keyboard
 *  shortcut, a toolbar icon can be set by the respective methods of
 *  QAction. A QCursor to appear when the action is aware can be set
 *  by setting the protected member cursor_.
 *  
 *  The mechanism is the following: 
 *
 *  The action gets active by the slot getActive(). First the action
 *  announces itself at the Controler. Then the virtual function
 *  handleSelection is called, for actions, that want to do something
 *  without user interaction. If a user click comes, the action is
 *  asked whether it will handle the DrawObject (that also can be a
 *  nullpointer) by wouldHandle().  If yes click() is called that gets
 *  the DrawObject and the point clicked at as parameter.
 *
 *  At any time the action can call Controler::execAction() and pass
 *  the Controler an instance of a Command that the controler will
 *  execute.
 */

#include "interactiveaction.h"
#include "drawobject.h"
#include "controler.h"

#include <QDebug>

/** The constructor takes a Controler and a QActionGroup as a parameter.
 * 
 */
InteractiveAction::InteractiveAction( Controler* parent, QActionGroup* group )
        : QAction( parent ),
          controler_( parent ),
          selection_( controler_->selection() )
{
        setActionGroup( group );
        connect( this, SIGNAL( triggered() ), this, SLOT( getActive() ) );
        connect( &selection_, SIGNAL( selectionChanged() ), this, SLOT( selectionChanged() ) );
}

/** 
 * Will announce the action to the Controler. Usually called by the
 * signal triggered().
 */
void InteractiveAction::getActive()
{
        reset();
        controler_->newAction( this ); 

        if ( !( selection_.isEmpty() || wouldHandleSelection() ) )
                selection_.select( 0 );

        if ( wouldHandleSelection() ) 
                handleSelection();        
}

void InteractiveAction::selectionChanged()
{
//        qDebug() << "selectionChanged" << this;
        setEnabled( wouldHandleSelection() );
}

/** Returns true, if the action handles the DrawObject o in case the
 * user clicks at point p. If o is the same object that is already
 * handled we always return true.
 */
bool InteractiveAction::wouldHandleSelection( const QPoint& p, const QMatrix* m ) 
{
        if ( selection_.isEmpty() )
                return wouldHandle( 0, p );

        foreach ( DrawObject* o, selection_.objects() )
                if ( wouldHandle( o, p, m ) )
                        return true;

        return false;

}

const QString InteractiveAction::completeName() const
{
        return tr("%1 %2").arg( commandName() ).arg( tr(selection_.objectname()) );
}
