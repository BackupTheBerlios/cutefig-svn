 
/*****************************************************************************
**
**  $Id$
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

#ifndef textaction_h
#define textaction_h

#include "createaction.h"

class TextObject;

class TextAction : public CreateAction
{
public:
        TextAction( Controler* parent, QActionGroup* group = 0 )
                : CreateAction( parent, group ),
                  pointIsSet_( false )
        {
                setText( tr("Create &Text") );
                setIcon( QIcon(":images/text.png") );
                setShortcut( Qt::Key_T );
                cursor_ = Qt::IBeamCursor;
                controler_->setTextAction( this );
        }
        
        ~TextAction() {}

        virtual void click( const QPoint& p, Fig::PointFlag f, const QMatrix* m );
        virtual void move( const QPoint&, const QMatrix* ) {}

        virtual bool keyStroke( const QKeyEvent* ke );
        
//        virtual const QCursor cursor( DrawObject* ) const { return Qt::IBeamCursor; }

//        virtual const QString commandName() const;

        virtual bool wouldHandle( DrawObject*, const QPoint& = QPoint(), const QMatrix* =0 )
        {
                return !pointIsSet_;
        }

        bool isEditing() const
        {
                return pointIsSet_;
        }

        TextObject* textObject() { return textObject_; }

        virtual bool acceptsKeyStrokes() { return pointIsSet_; }

        virtual void reset();
        
protected:
        virtual DrawObject* createObject();

        bool event( QEvent* e );
        
private:
        bool pointIsSet_;
        TextObject* textObject_;
        int cursorTimer_;
};

#endif
