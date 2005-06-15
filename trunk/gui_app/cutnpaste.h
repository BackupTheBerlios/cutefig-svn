 
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

#ifndef cutnpaste_h
#define cutnpaste_h

#include "interactiveaction.h"

class Clipboard;

class CopyAction : public InteractiveAction 
{
        Q_OBJECT
public:
        CopyAction( Controler* parent, QActionGroup* group = 0 );
        ~CopyAction() {}

        void click( const QPoint& p,Fig::PointFlag f, const QMatrix* m );
        virtual bool isActive() const { return false; }

        virtual bool wouldHandle( DrawObject* o, const QPoint& = QPoint(), const QMatrix* =0 ) { 
                return o; 
        }

        virtual void handleSelection();

        virtual const QString commandName() const { return QString(); }

private:
        Clipboard* clipboard_;
};

class PasteAction : public InteractiveAction
{
        Q_OBJECT
public:
        PasteAction( Controler* parent, QActionGroup* group = 0 );
        ~PasteAction() {}

        void click( const QPoint&, Fig::PointFlag, const QMatrix* ) {}

        virtual bool isActive() const { return false; }

        virtual bool wouldHandleSelection( const QPoint& = QPoint(), const QMatrix* = 0 );

        virtual bool wouldHandle( DrawObject*, const QPoint& = QPoint(), const QMatrix* =0 ) { 
                return true; 
        }
        
        virtual void handleSelection();

        virtual const QString commandName() const { return "paste"; }

private:
        Clipboard* clipboard_;
};

class CutAction : public InteractiveAction
{
        Q_OBJECT
public:
        CutAction( Controler* parent, QActionGroup* group = 0 );
        ~CutAction() {}

        void click( const QPoint& p,Fig::PointFlag f, const QMatrix* m );
        virtual bool isActive() const { return false; }

        virtual bool wouldHandle( DrawObject* o, const QPoint& = QPoint(), const QMatrix* =0 ) { 
                return o; 
        }

        virtual void handleSelection();

        virtual const QString commandName() const { return "cut"; }

private:
        Clipboard* clipboard_;
};

#endif
