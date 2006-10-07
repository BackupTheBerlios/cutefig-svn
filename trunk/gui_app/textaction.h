 
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
class QTextCharFormat;

class TextAction : public CreateAction
{
        Q_OBJECT
public:
        TextAction( Controler* parent );
        ~TextAction() {}

        virtual void click( const QPoint& p, Fig::PointFlags f, const QMatrix& m );
        virtual void move( const QPoint&, const QMatrix& ) {}

        virtual bool keyStroke( const QKeyEvent* ke );
        virtual bool inputMethodEvent( const QInputMethodEvent* e );
        
//        virtual const QCursor cursor( DrawObject* ) const { return Qt::IBeamCursor; }

        virtual const QString commandName() const;

        virtual bool wouldHandle( DrawObject*, const QPoint& = QPoint(), const QMatrix* =0 )
        {
                return true;
        }

        virtual void handleSelection() {}

        bool isEditing() const { return textObject_; }

        TextObject* textObject() { return textObject_; }

        virtual bool acceptsKeyStrokes() { return textObject_; }

        virtual bool wantsSnap( const QPoint& p, const QMatrix& m );

        virtual void reset();
        
protected:
        virtual DrawObject* createObject() { return 0; }
        virtual void setInitialStatus() {}
        virtual void changeStatusClick() {}
        virtual void changeStatusMove() {}
        
private slots:
        void dispatchFormat();

private:
        TextObject* textObject_;
        int cursorTimer_;
        bool textNew_;

        bool event( QEvent* e );
        bool cursorMovement( const QKeyEvent* ke );
        void commitTextObject();

        TextObject* textObjectUnderPoint( const QPointF& p );
};



#endif
