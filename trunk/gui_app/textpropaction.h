 
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

#ifndef textpropaction_h
#define textpropaction_h

#include "interactiveaction.h"

class TextObject;
class TextAction;

class TextPropAction : public InteractiveAction
{
public:
        TextPropAction( Controler* parent, QActionGroup* = 0 );
        ~TextPropAction() {}

        virtual void click( const QPoint&, Fig::PointFlag, const QMatrix* );

        virtual void handleSelection();
        
        virtual bool isActive() const { return false; }

        virtual bool wouldHandle( DrawObject* o, const QPoint& p= QPoint(), const QMatrix* m=0 );

protected:
        virtual void handleObject( TextObject* to ) = 0;
        virtual void wakeup();
        
private:
        TextAction* textAction_;
};


class TextHCenterAction : public TextPropAction
{
public:
        TextHCenterAction( Controler* parent, QActionGroup* agr = 0 )
                : TextPropAction( parent, agr )
        {
                setText( tr("Align &center") );
                setIcon( QIcon(":images/text_center.png") );
        }

        virtual void handleObject( TextObject* to );

        virtual const QString commandName() const
        {
                return tr("center");
        }        
};


class TextLeftAction : public TextPropAction
{
public:
        TextLeftAction( Controler* parent, QActionGroup* agr = 0 )
                : TextPropAction( parent, agr )
        {
                setText( tr("Align &left") );
                setIcon( QIcon(":images/text_left.png") );
        }

        virtual void handleObject( TextObject* to );

        virtual const QString commandName() const
        {
                return tr("align left");
        }        
};


class TextRightAction : public TextPropAction
{
public:
        TextRightAction( Controler* parent, QActionGroup* agr = 0 )
                : TextPropAction( parent, agr )
        {
                setText( tr("Align &right") );
                setIcon( QIcon(":images/text_right.png") );
        }

        virtual void handleObject( TextObject* to );

        virtual const QString commandName() const
        {
                return tr("align right");
        }        
};


#endif