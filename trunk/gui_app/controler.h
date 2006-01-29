 
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
 *  \brief The Controler is the connection between the views
 *  (ViewBase) and the Figure.
 */
#ifndef controler_h
#define controler_h

#include "fig.h"
#include "command.h"
#include "viewbase.h"
#include "pen.h"
#include "selection.h"

#include <QPointF>
#include <QCursor>
#include <QMainWindow>
#include <QStringList>
#include <QColor>
#include <QList>
//#include <QVector>
#include <QPolygonF>

class Figure;
class DrawObject;
class ActionCollection;
class InteractiveAction;
class EditActions;
class ToolActions;
class TextAction;
class TextPropActions;
class QEvent;

typedef QList<ActionCollection*> ActionGroups;

class Controler : public QObject
{
        Q_OBJECT
public:
        Controler( QMainWindow * parent =0 );
        ~Controler() { }

        void setFigure( Figure* f );
        Figure* figure() const { return figure_; }
        QMainWindow* mainWindow() { return mainWindow_; }

        void clearCommandList();
        void resetFigure();
        
        void addView( ViewBase* v ) { viewList_.push_back( v ); }

        void setEditActionsGroup( EditActions* ea ) { editActionsGroup_ = ea; }
        void setToolActionsGroup( ToolActions* ta );
        ToolActions* toolActions() const { return toolActionsGroup_; }

        void setTextAction( TextAction* ta ) { textAction_ = ta; }
        TextAction* textAction() const { return textAction_; }

        TextPropActions* textPropActions() const { return textPropActions_; }
        void setTextPropActions( TextPropActions* tpa ) { textPropActions_ = tpa; }
        //EditActions* editActionsGroup() const { return editActionsGroup_; }

        void selectObject( DrawObject* o );
        
        const ObjectList& selectedObjects() { return selection_.objects(); }
        const ObjectList& backups() { return selection_.backups(); }
        Selection& selection() { return selection_; }

        const QCursor considerObject( DrawObject* o, const QPoint& p, const QMatrix* m ) const;
        const QCursor findToolAction( const QPoint& p, const QMatrix* m );

        const QPolygonF objectsPoints();

        bool hasAction() const { return editAction_; }
        bool willAcceptClick( const QPoint& p, const QMatrix* m ) const;
        bool wouldAcceptClick( const QPoint&, const QMatrix* m ) const;

        bool willAcceptKeyStroke() const;

        bool actionWantsSnap( const QPoint& p, const QMatrix* m ) const;
        
        void callActionMove( const QPoint& p, const QMatrix* m );
        const QCursor callActionClick( const QPoint& p, Fig::PointFlag f, const QMatrix* m );

        bool callActionKeyStroke( const QKeyEvent* ke );

        bool callInputMethodHandler( const QInputMethodEvent* e );
        
        void cancelAction();
        void execAction( Command* cmd );
        
        bool figureChanged() const { return figureChanged_; }
        void figureSaved() { figureChanged_ = false; }

        ActionGroups actionGroups() const { return actionGroups_; }
        void addActionGroup( ActionCollection* ag ) { actionGroups_.append( ag ); }

        void updateViews();

        void drawActionMetaData( QPainter* p, const ViewBase* v );
        
signals:
        void undoPossible( bool );
        void redoPossible( bool );

public slots:
        void undo();
        void undo( int steps );

        void redo();
        void redo( int steps );
  
        void newAction( InteractiveAction* a );
        
//         void setPenWidth( const qreal& pw ) { toolPen_.setWidth( pw ); }
//         void setPenColor( const QColor& pc ) { toolPen_.setColor( pc ); }
    
//         void setFillColor( const QColor& c ) { toolFillColor_ = c; }

        void setDepth( int v ) { toolDepth_ = v; }


private:
        void updateViewsImediately( bool tentative = false);
        virtual bool event( QEvent* e );

        void setToolProperties();
        void appendToCmdList( Command* c );

        Figure* figure_;
        QMainWindow* mainWindow_;

        InteractiveAction* editAction_;
        bool actionIsActive_, explicitEAction_;

        QVector<ViewBase*> viewList_;

        typedef QList<Command*> CommandStack;
        CommandStack commandStack_;
        typedef QMutableListIterator<Command*> CmdStackIt;
        CmdStackIt currentCommand_;

        Selection selection_;
//        DrawObject *wObject_, *backup_;
        bool figureChanged_;

        Pen toolPen_;
        QColor toolFillColor_;
        int toolDepth_;

        typedef QVector<InteractiveAction*> activeToolActions_t;
        activeToolActions_t activeToolActions_;

        EditActions* editActionsGroup_;
        ToolActions* toolActionsGroup_;

        TextAction* textAction_;
        TextPropActions* textPropActions_;
        
        ActionGroups actionGroups_;
};


#endif
