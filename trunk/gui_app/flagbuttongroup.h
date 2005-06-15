 
/*****************************************************************************
**
**  @version $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2005 Johannes Mueller, joh@users.berlios.net
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

#ifndef flagbuttongroup_h
#define flagbuttongroup_h

#include <QFlags>
#include <QButtonGroup>
#include <QSignalMapper>
//#include <QMap>

class QAbstractButton;

class FlagButtonGroup : public QButtonGroup
{
        Q_OBJECT
public:
        FlagButtonGroup( QWidget* parent );
        ~FlagButtonGroup() {}

        bool addButton( QAbstractButton* b, int id );

        int state() const { return state_; }

signals:
        void stateChanged( int );

public slots:
        void setState( int state );

private:
        int state_;
//        QMap<int, QAbstractButton*> buttonMap_;

        QSignalMapper signalMapper_;

private slots:
        void changeState( int id );
};

#endif
