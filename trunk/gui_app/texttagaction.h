 
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

#ifndef texttagaction_h
#define texttagaction_h

#include "textpropaction.h"
#include "textobject.h"

class TextTagAction : public TextPropAction
{
        Q_OBJECT
protected:
        TextTagAction(  Controler* parent );
public:
        ~TextTagAction() {}

        virtual void click( const QPoint&, Fig::PointFlags, const QMatrix& ) {}
        virtual void handleSelection() {}
        virtual bool isActive() { return false; }

        virtual bool wouldHandle( DrawObject*, const QPoint& = QPoint(), const QMatrix* = 0 );

        virtual void adaptToTextFormat();
        
private:
        virtual void handleObject( TextObject* ) {}
        virtual void wakeup();

        virtual bool correctState( const TextObject* to ) = 0;
        virtual void toggleState( TextObject* to ) = 0;        
};


class TextBoldAction : public TextTagAction
{
public:
        TextBoldAction( Controler* parent );
        virtual const QString commandName() const { return tr("bold"); }
        
private:
        virtual bool correctState( const TextObject* to );
        virtual void toggleState( TextObject* to );
};

class TextItalicAction : public TextTagAction
{
public:
        TextItalicAction( Controler* parent );
        virtual const QString commandName() const { return tr("italic"); }
        
private:
        virtual bool correctState( const TextObject* to );
        virtual void toggleState( TextObject* to );
};

class TextSuperScriptAction : public TextTagAction
{
public:
        TextSuperScriptAction( Controler* parent );
        virtual const QString commandName() const { return tr("superscript"); }

private:
        virtual bool correctState( const TextObject* to );
        virtual void toggleState( TextObject* to );
};


class TextSubScriptAction : public TextTagAction
{
public:
        TextSubScriptAction( Controler* parent );
        virtual const QString commandName() const { return tr("subscript"); }
        
private:
        virtual bool correctState( const TextObject* to );
        virtual void toggleState( TextObject* to );
};


#endif
