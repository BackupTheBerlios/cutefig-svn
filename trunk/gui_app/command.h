 
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

#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>

#include "selection.h"

class Controler;
class Figure;

class Command : public QObject
{
        Q_OBJECT
public: 
        Command( const Selection& s ) : QObject(), objects_( s.objects() ) {}
        ~Command() {};

        virtual void execute() = 0;
        virtual void unexecute() = 0;

        void setCommandName( const QString& s ) { commandString_ = s; }
        const QString commandString() const { return commandString_; }

        void setControler( Controler* cnt );

protected:
        ObjectList objects_;
        Controler* controler_;
        Figure* figure_;

private:
        QString commandString_;
};

#endif
