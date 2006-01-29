 
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

/** \class InteractiveAction
 *
 *  \brief Abstract baseclass for all unseractions, that need mouse
 *  interaction
 */

#include <QAction>

#include "fig.h"
#include "controler.h"

class DrawObject;
class Selection;
class ViewBase;

class QKeyEvent;

class InteractiveAction : public QAction
{
        Q_OBJECT
public:
        InteractiveAction( Controler* parent );
        ~InteractiveAction() {}

        virtual void click( const QPoint& p, Fig::PointFlag f, const QMatrix* m ) = 0;
        virtual void move( const QPoint&, const QMatrix* ) {}
        virtual bool keyStroke( const QKeyEvent* ) { return false; }
        virtual bool inputMethodEvent( const QInputMethodEvent* ) { return false; }
        
        virtual void handleSelection() {}

        virtual bool isActive() const = 0;
        virtual bool wouldHandle( DrawObject* o, const QPoint& p=QPoint(), const QMatrix* m=0 )=0;
        virtual bool wouldHandleSelection( const QPoint& p = QPoint(), const QMatrix* m = 0 );
        virtual bool wantsSnap( const QPoint&, const QMatrix* ) { return true; }
        virtual bool acceptsKeyStrokes() { return false; }

        virtual void drawMetaData( QPainter*, const ViewBase* ) const {}
        
        const QCursor& cursor() const { return cursor_; }

        virtual const QString commandName() const = 0;
        virtual const QString completeName() const;

        //! resets the action. (called by wakeup())
        /*! supposed to do cleanups */
        virtual void reset() {}

protected:
        Controler* controler_; //!< the Controler instance
        Selection& selection_; //!< the selection_

        QCursor cursor_;       //!< the cursor to appear during the action

protected slots:
        void selectionChanged();
        virtual void wakeup(); 
};

#endif
