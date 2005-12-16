 
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

#ifndef actioncollection_h
#define actioncollection_h

#include <QObject>
#include <QList>
#include <QHash>
#include <QAction>

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
 *     - A QHash to map a certain action to a QWidget in case the
 *       action wants to setup a special widget for the toolBar. This
 *       is done for example by undo and redo.
 */

class ActionCollection : public QObject
{
public:
        ActionCollection( QObject* parent )
                : QObject( parent ),
                  specialWidgets_(),
                  actions_()
        {}

        ~ActionCollection() {}

        void addSeparator(); //!< adds a seperator to appear in the menu
        const QString text() { return text_; } //!< returns the name to appear on the menu bar.

        QWidget* toolBarWidget( QAction* action ) { return specialWidgets_[action]; }
        //!< returns a pointer to a special QWidget or 0 if the action does not have one.

        void setAllEnabled( bool enabled = true ); //!< probably not needed

        const QList<QAction*>& actions() const { return actions_; }

        void addAction( QAction* a ) { actions_ << a; }
        
protected:        
        void setText( const QString& text ) { text_ = text; } 
        //!< sets the text to appear in the menu bar.

        QHash<QAction*, QWidget*> specialWidgets_;

private:
        QList<QAction*> actions_;
        //!< in this QHash actions can store their special buttons

        QString text_;
};


#endif
