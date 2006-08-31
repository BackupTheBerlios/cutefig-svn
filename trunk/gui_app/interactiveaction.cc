 
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


#include "interactiveaction.h"
#include "drawobject.h"
#include "geometry.h"
#include "controler.h"
#include "viewbase.h"

#include <QCoreApplication>

#include <QDebug>

/** The constructor takes a Controler a parameter.
 * 
 */
InteractiveAction::InteractiveAction( Controler* parent )
        : QAction( parent ),
          controler_( parent ),
          selection_( controler_->selection() ),
          status_()
{
        connect( this, SIGNAL( triggered() ), this, SLOT( wakeup() ) );
        connect( &selection_, SIGNAL( selectionChanged() ), this, SLOT( selectionChanged() ) );
}

/** 
 * Will announce the action to the Controler. Usually called by the
 * signal triggered(). Can be reimplemented by actions that do not
 * want to register themselves to the Controler in every case. Any
 * TextPropAction is such a case.
 */
void InteractiveAction::wakeup()
{
        reset();
        controler_->newAction( this ); 

        if ( !( selection_.isEmpty() || wouldHandleSelection() ) )
                selection_.select( 0 );

        setInitialStatus();
        
        if ( wouldHandleSelection() ) 
                handleSelection();

        emit statusChanged( status_ );
}

void InteractiveAction::setInitialStatus()
{
        status_.clear();
        setInitialStatus_private();
}

void InteractiveAction::modifierChange( Qt::KeyboardModifiers mods )
{
        if ( mods & Qt::MetaModifier ) { 
                mods ^= Qt::MetaModifier;
                mods ^= Qt::AltModifier;
        }

        if ( mods & Qt::KeypadModifier )
                mods ^= Qt::KeypadModifier;
        
        status_.setModifiers( mods );
        modifierChange_private( mods );
        emit statusChanged( status_ );
}

void InteractiveAction::selectionChanged()
{
        bool enable = wouldHandleSelection();
        setEnabled( enable );

        if ( !enable )
                setChecked( false );
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
        return tr("%1 %2").arg( commandName() ).arg( selection_.objectname() );
}




