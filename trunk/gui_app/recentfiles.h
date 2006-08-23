 
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

#ifndef recentfiles_h
#define recentfiles_h

#include <QHash>
#include <QObject>

class ActionCollection;
class CuteFig;

class QActionGroup;

//! Maintains instances of QAction for FileActions, that will load recently visited files.
/** RecentFiles is implemented as a singleton whose instance is not
 *  accessible directly but only through the two static functions
 *  getActions() and addFile().
 *
 *  The interface is easy: An ActionCollection e.g. FileActions can
 *  have added the QActions by calling getActions() statically passing
 *  itself and a pointer to the CuteFig instance, that is to open the
 *  files when one of the QActions are triggered. Secondly the static
 *  method addFile() is to be calles (e.g. by CuteFig) as soon as a
 *  new file name comes into play.
 *
 *  The mechanism is a litte bit more complicated as CuteFig is for
 *  the time being constructed as a SDI application. So for every
 *  instance of CuteFig a set of QActions is maintained in a
 *  QActionGroup. These QActionGoups are then stored in a QHash
 *  accessible by actionGroups(). All this is neccessary to update the
 *  QActions when a new file name is handled somewhere and that the
 *  appropriate QActionGroup can be removed when an instance of
 *  CuteFig and thus FileActions is deleted.
 */
class RecentFiles : public QObject
{
        Q_OBJECT
public:
        static void getActions( ActionCollection* ac, CuteFig* cuteFig );
        static void addFile( const QString& fn );
                
private:
        static RecentFiles* instance();
        RecentFiles( const RecentFiles& );
        RecentFiles() {}

        static void updateActions( QActionGroup* ag, const QStringList& files );
        static void updateActionGroups();

private slots:
        void collectionDeleted( QObject* ob = 0 );

private:
        static const int maxRecentFiles;
        static QHash<QObject*, QActionGroup*>& actionGroups();

        class SignalDispatcher;
};


//! A small helper class to translate signal QAction::triggered() to a function call to CuteFig
/** One problem in RecentFiles is, that a QAction has only the signal
 *  triggered() that can not parse an argument. So translateSignal()
 *  extracts the file name to open out of QAction::data() and calls
 *  then CuteFig::load().
 */
class RecentFiles::SignalDispatcher : public QObject 
{
        Q_OBJECT
public:
        SignalDispatcher( CuteFig* parent );

public slots:
        void translateSignal();

private:
        CuteFig* cuteFig_;
};


#endif
