 
/*****************************************************************************
**
**  @version $Id: valuehash.h 101 2006-01-12 19:18:11Z joh $
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

#ifndef vhahsmenu_h
#define vhahsmenu_h

#include "valuehash.h"

#include <QMenu>
#include <QList>
#include <QActionGroup>


class AbstractHashMenu : public QObject
{
        Q_OBJECT
public:
        AbstractHashMenu( QObject* parent );

signals:
        void valueChanged( QString name );

protected:
        void setupActions( const QList<QString> names, const QString& initial );

        QActionGroup actions_;
};


template<typename ValueType> class ValueHashMenu
{
public:
        static ValueHash<ValueType> getValue( const ValueHash<ValueType>& initial ); 

private:
};


template<typename ValueType>
ValueHash<ValueType> ValueHashMenu<ValueType>::getValue( const ValueHash<ValueType>& initial )
{
        QList<QString> names = ValueHash<ValueType>::names();

        if ( names.isEmpty() )
                return ValueHash<ValueType>();

        QMenu menu;

        QList<QAction*> actions;
        QAction* at = 0;
        
        foreach ( QString n, names ) {
                QAction* a = new QAction( n, &menu );
                a->setCheckable( true );
                if ( initial.name() == n ) {
                        a->setChecked( true );
                        at = a;
                }
                actions << a;
        }
        
        QAction* res = menu.exec( actions, QCursor::pos(), at );

        return ValueHash<ValueType>( names[actions.indexOf( res )] );
}

                                          

#endif
