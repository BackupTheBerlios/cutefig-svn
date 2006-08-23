 
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

#include "recentfiles.h"
#include "actioncollection.h"
#include "cutefig.h"

#include <QSettings>
#include <QAction>
#include <QStringList>
#include <QFileInfo>

const int RecentFiles::maxRecentFiles = 5;


static QStringList fileList()
{
        QSettings s;
        return s.value("recentFiles").toStringList();
}

static QString baseName( const QString& fullname )
{
        return QFileInfo( fullname ).fileName();
}


RecentFiles* RecentFiles::instance()
{
        static RecentFiles rf;
        return &rf;
}


void RecentFiles::getActions( ActionCollection* ac, CuteFig* cuteFig )
{
        QActionGroup* agr = new QActionGroup( ac );

        SignalDispatcher* sd = new SignalDispatcher( cuteFig );
                
        for ( int i = 0; i < maxRecentFiles; ++i ) {
                QAction* a = new QAction( ac );
                a->setVisible( false );
                connect( a, SIGNAL( triggered() ), sd, SLOT( translateSignal() ) );
                ac->addAction( a );
                agr->addAction( a );
        }

        QAction* sep = new QAction( ac );
        sep->setSeparator( true );
        ac->addAction( sep );
        agr->addAction( sep );

        actionGroups()[ac] = agr;
        connect( ac, SIGNAL( destroyed(QObject*) ),
                 instance(), SLOT( collectionDeleted(QObject*) ) );

        updateActions( agr, fileList() );
}


void RecentFiles::addFile( const QString& fn )
{
        QStringList files = fileList();
        files.removeAll(fn);
        files.prepend(fn);
        if ( files.size() > maxRecentFiles )
                files.removeLast();

        QSettings s;
        s.setValue("recentFiles", files );

        updateActionGroups();
}


void RecentFiles::updateActions( QActionGroup* ag, const QStringList& files )
{
        int n = qMin( files.size(), maxRecentFiles );

        for ( int i = 0; i < n; ++i ) {
                QAction* a = ag->actions()[i];
                a->setText( tr("&%1 %2").arg( i+1 ).arg( baseName( files[i] ) ) );
                a->setData( files[i] );
                a->setVisible( true );
        }

        ag->actions().last()->setVisible( n );
}

void RecentFiles::updateActionGroups()
{
        const QStringList files = fileList();
        foreach( QActionGroup* ag, actionGroups() )
                updateActions( ag, files );
}


void RecentFiles::collectionDeleted( QObject* ob )
{
        actionGroups().remove( ob );
}


QHash<QObject*, QActionGroup*>& RecentFiles::actionGroups()
{
        static QHash<QObject*, QActionGroup*> h;
        return h;
}


RecentFiles::SignalDispatcher::SignalDispatcher( CuteFig* parent )
        : QObject( parent ),
          cuteFig_( parent )
{
}

void RecentFiles::SignalDispatcher::translateSignal()
{
        QAction* a = qobject_cast<QAction*>( sender() );
        if ( a )
                cuteFig_->load( a->data().toString() ); 
}
