 
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

#ifndef controler_h
#define controler_h

#include "fig.h"
#include "command.h"
#include "viewbase.h"
#include "selection.h"
#include "interactiveaction.h"

#include <QPointF>
#include <QCursor>
#include <QMainWindow>
#include <QStringList>
#include <QColor>
#include <QList>
#include <QPolygonF>

class Figure;
class DrawObject;
class ActionCollection;
class EditActions;
class ToolActions;
class TextAction;
class TextPropActions;
class QEvent;

typedef QList<ActionCollection*> ActionGroups;


//! The Controler is the connection between the views (ViewBase) and the Figure.
/*! A Controler handles all the user interaction and manipulates the
 *  Figure. After it changed the Figure all the views of viewList_ get
 *  the ViewBase::updateFigure() command. The Controler keeps track of
 *  all user interaction by storing all the commands in the
 *  commandStack_. The commandStack_ is used by the undo() and redo()
 *  functions that are called by the coresponding actions.
 *
 *  \section lazysnap_set Interaction with CanvasView
 */
class Controler : public QObject
{
        Q_OBJECT
public:
        Controler( QMainWindow * parent =0 );
        ~Controler();

        void setFigure( Figure* f );
        Figure* figure() const { return figure_; }
        QMainWindow* mainWindow() { return mainWindow_; }

        void clearCommandList();
        void resetFigure();

        void addView( ViewBase* v ) { viewList_.push_back( v ); }

        void setEditActionsGroup( EditActions* ea ) { editActionsGroup_ = ea; }
        void setToolActionsGroup( const ToolActions* ta );
        const ToolActions* toolActions() const { return toolActionsGroup_; }

        void setTextAction( TextAction* ta ) { textAction_ = ta; }
        TextAction* textAction() const { return textAction_; }

        TextPropActions* textPropActions() const { return textPropActions_; }
        void setTextPropActions( TextPropActions* tpa ) { textPropActions_ = tpa; }

        void selectObject( DrawObject* o );
        
        const ObjectList& selectedObjects() { return selection_.objects(); }
        const ObjectList& backups() { return selection_.backups(); }
        Selection& selection() { return selection_; }

        const QCursor considerObject( DrawObject* o, const QPoint& p, const QMatrix& m ) const;
        const QCursor findToolAction( const QPoint& p, const QMatrix& m );

        const QPolygonF objectsPoints();

        bool hasAction() const { return editAction_; }

        //! Can be used to ask the Controler if it will accept the click the user just made.
        bool willAcceptClick( const QPoint& p, const QMatrix& m ) const;

        //! Can be used to ask the Controler if potential click right now would be accepted.
        bool wouldAcceptClick( const QPoint&, const QMatrix& m ) const;

        bool willAcceptKeyStroke() const;
        bool actionIsActive() const { return actionIsActive_; }
        bool actionWantsSnap( const QPoint& p, const QMatrix& m ) const;

        //! Called to pass a mouse move to the editAction_. 
        void callActionMove( const QPoint& p, const QMatrix& m );

        //! Called to pass a mouse click to the editAction_.
        const QCursor callActionClick( const QPoint& p, Fig::PointFlags f, const QMatrix& m );

        //! Called to pass a key stroke to the editAction_.
        bool callActionKeyStroke( const QKeyEvent* ke );

        //! Called to pass an input event to the editAction_ (needed for text input)
        bool callInputMethodHandler( const QInputMethodEvent* e );

        void modifierChange( Qt::KeyboardModifiers mods );
        
        void cancelAction();
        void execAction( Command* cmd );

        bool figureChanged() const { return figureChanged_; }
        void figureSaved() { figureChanged_ = false; }

        ActionGroups actionGroups() const { return actionGroups_; }
        void addActionGroup( ActionCollection* ag ) { actionGroups_.append( ag ); }

        void drawActionMetaData( QPainter* p, const ViewBase* v );
        
signals:
        void undoPossible( bool );
        void redoPossible( bool );

        void actionStatusChanged( const ActionStatus& st );
        void actionIsAway( bool );

public slots:
        void undo( int steps );
        void redo( int steps );
  
        void newAction( InteractiveAction* a );

        void updateViews();

        void updateFigureMetaData() const;

        void setDepth( int v ) { toolDepth_ = v; }


private:
        void updateViewsImediately( bool tentative = false);
        virtual bool event( QEvent* e );

        void appendToCmdList( Command* c );

        void actionGone();

        Figure* figure_;
        QMainWindow* mainWindow_;

        //! The InteractiveAction that is cuurently in use.
        InteractiveAction* editAction_;

        //! true if the editAction_ has started its operation
        bool actionIsActive_;

        //! true if the editAction_ has been chosen by the user and not by findToolAction()
        bool explicitEAction_;

        QVector<ViewBase*> viewList_;

        typedef QList<Command*> CommandStack;
        CommandStack commandStack_;
        typedef QMutableListIterator<Command*> CmdStackIt;
        CmdStackIt currentCommand_;

        Selection selection_;
        bool figureChanged_;

        int toolDepth_;

        typedef QVector<InteractiveAction*> activeToolActions_t;
        activeToolActions_t activeToolActions_;

        EditActions* editActionsGroup_;
        const ToolActions* toolActionsGroup_;

        TextAction* textAction_;
        TextPropActions* textPropActions_;
        
        ActionGroups actionGroups_;
};


#endif
