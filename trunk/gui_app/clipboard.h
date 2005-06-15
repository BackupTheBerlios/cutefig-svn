 
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

#ifndef clipboard_h
#define clipboard_h

#include <QObject>
#include <QApplication>

#include <selection.h>

class Clipboard : public QObject
{
        Q_OBJECT

        Clipboard() 
                : QObject( QApplication::instance() ),
                  contents_() {}
public:
        ~Clipboard() {}

        static Clipboard* instance()
        {
                static Clipboard* inst = new Clipboard();
                return inst; 
        }

        void setContents( const ObjectList& l ) 
        { 
                contents_ = l; 
                emit hasContents( !l.isEmpty() );
        }
        
        const ObjectList& contents() const { return contents_; }

signals:
        void hasContents( bool );

private:
        ObjectList contents_;
};

#endif
