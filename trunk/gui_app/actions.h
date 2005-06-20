 
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

#include <QAction>
#include <QActionGroup>
#include <QList>
#include <QHash>

class QToolButton;
class QSignalMapper;

class Controler;
class CuteFig;
class ActionCollection;
class CanvasView;
class InteractiveAction;

typedef QList<ActionCollection*> ActionGroups;


/** \page action_system The action system
 *
 * Almost all the user interaction is initiated by actions. An action
 * is a class that is derived from QAction. There ist one abstract
 * action class for actions that directly modify the figure,
 * InteractiveAction.
 *
 * Actions that are usually together in one menu in the menubar are to
 * put into one ActionCollection. All the ActionCollections are to be
 * registered into AllActions. A way to do that is still to be
 * implemented. So far AllActions constructs those itself.
 *
 * \link CuteFig::setupActions() CuteFig::setupActions() \endlink
 * creates the menus for the menubar and toolbars for the main window.
 *
 */
/** \class ActionCollection 
 *
 * \brief Actions belonging semantically * together are put into one
 * ActionCollection
 *
 * For every menu in the menubar and its submenus there is one
 * ActionCollection. ActionCollection is derived from QActionGroup.
 * It brings in the following functionality:
 *     - the text() attribute, the name of the menu in the menu bar
 *     - the addSeparator() method as suggested in the QT4-docs to 
 *       replace addSeperator() of QActionGroup in QT3
 *     - A QHash to map a certain action to a QToolButton in case the
 *       action wants to setup a special button for the toolBar. This
 *       is done for example by undo and redo.
 */

class ActionCollection : public QActionGroup
{
        Q_OBJECT
public:
        ActionCollection( QObject* parent ) 
                : QActionGroup( parent ),
                  specialButtons_() {}

        ~ActionCollection() {}

        void addSeparator(); //!< adds a seperator to appear in the menu
        const QString text() { return text_; } //!< returns the name to appear on the menu bar.

        QToolButton* toolButton( QAction* action ) { return specialButtons_[action]; }
        //!< returns a pointer to a special QToolbutton or 0 if the action does not have one.

        void setAllEnabled( bool enabled = true ); //!< probably not needed

protected:        
        void setText( const QString& text ) { text_ = text; } 
        //!< sets the text to appear in the menu bar.

        QHash<QAction*, QToolButton*> specialButtons_;
        //!< in this QHash actions can store their special buttons

private:
        QString text_;
};


/** \class AllActions
 *  \brief Contains all actions of the application.
 *
 *  At the moment AllActions sets up all the actions that are
 *  known. However there will be a mechanism to register new actions
 *  at runtime coming from extensions.
 *
 *  actionGroups() returns the list of all registered ActionCollections.
 */
class AllActions : public QObject
{
        Q_OBJECT
public:
        AllActions( CuteFig* parent );
        ~AllActions() {}

        ActionGroups actionGroups() const { return actionGroups_; }

private:
        void addActions( ActionCollection* actions ) { actionGroups_.append( actions ); }
        ActionGroups actionGroups_;
};


/******************************************************************************************/

/** \class FileActions
 *  \brief All the actions of the "File" menu.
 */
class FileActions : public ActionCollection
{
        Q_OBJECT
public:
        FileActions( CuteFig* parent );
        ~FileActions() {}
};


/** \class EditActions
 *  \brief All the actions of the "Edit" menu
 *
 *  This class has the special functionality to implement an undo and
 *  redo menu. It therefore sets special QToolButtons to be put into
 *  the specialButtons_ QHash. The QSignalMappers undoSignalMapper_
 *  and redoSignalMapper_ map the menu items to an int that is passed
 *  to the Controler.
 */
class EditActions : public ActionCollection
{
        Q_OBJECT
public:
        EditActions( Controler* parent );
        ~EditActions() {}

        void addCommand( const QString& s );
        //!< Called by the controler when a Command has been executed. 

        void undo( const QString& s );
        //!< Called by the controler when undo has been called

        void redo( const QString& s );
        //!< Called by the controler when redo has been called

private:
        QAction *undo_, *redo_;
        QAction *undoHead_, *redoHead_; //!< pointer to the first entry in the undo/redo menu
        QMenu *undoMenu_, *redoMenu_;
        QSignalMapper *undoSignalMapper_, *redoSignalMapper_;

        void remapUndoSignals(); //!< remap the menu when changed
        void remapRedoSignals(); //!< remap the menu when changed

        static const QString undoString_;
        static const QString redoString_;
        static const QString colonSep_;
};


/** \class ViewActions
 *  \brief All the actions of the View menu.
 */
class ViewActions : public ActionCollection
{
        Q_OBJECT
public:
        ViewActions( CanvasView* parent );
        ~ViewActions() {}
};

/** \class FormatActions
 *  \brief All the actions of the Format menu (like group/ungroup)
 */
class FormatActions : public ActionCollection 
{
        Q_OBJECT
public:
        FormatActions( Controler* parent );
        ~FormatActions() {}
};

/** \class ToolActions
 *  \brief All the actions that need mouse interaction
 *
 *  Pointers of the actions which must inherit InteractiveAction are
 *  exposed to the public by getter functions. This is for CanvasView
 *  to be able to activate such an action if necessary.
 */
class ToolActions : public ActionCollection
{
        Q_OBJECT
public:
        ToolActions( Controler* parent );
        ~ToolActions() {}

        InteractiveAction* move() const { return move_; }
        InteractiveAction* scale() const { return scale_; }

private:
        InteractiveAction* move_;
        InteractiveAction* scale_;
};


/** \class CreateActions
 *  \brief All the actions that create DrawObjects
 *
 *  They are not set up manually but by the list of ObjectMapper
 */
class CreateActions : public ActionCollection
{
        Q_OBJECT
public:
        CreateActions( Controler* parent );
        ~CreateActions() {}
};

