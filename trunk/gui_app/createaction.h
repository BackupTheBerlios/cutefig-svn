 
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

#ifndef createaction_h
#define createaction_h 

#include "interactiveaction.h"
#include "typedefs.h"
#include "controler.h"

/*! \class CreateAction
 *  \brief A base class for create actions of DrawObjects
 *
 *  Each DrawObject that can be createt from skratch must come up with
 *  a subclass of CreateAction.
 */

class CreateAction : public InteractiveAction
{
protected:
        CreateAction( Controler* parent ) 
                : InteractiveAction( parent ),
                  cObject_( 0 ),
                  firstClick_( false )
        {}
        
public:
        ~CreateAction() {}

        virtual void click( const QPoint& p, Fig::PointFlags f, const QMatrix& m );
        virtual void move( const QPoint& p, const QMatrix& m );

        virtual bool wouldHandle( DrawObject*, const QPoint& = QPoint(), const QMatrix* =0 )
        { return true; }

        virtual bool isActive() const { return true; }

        virtual const QCursor cursor( DrawObject* ) const { return Qt::PointingHandCursor; }
        
        virtual void handleSelection();

        virtual const QString commandName() const { return "create"; }

        virtual void drawMetaData( QPainter* p, const ViewBase* v ) const;

        virtual void reset();

        const DrawObject* drawObject() const { return cObject_; }

protected:
        bool firstClickDone() const { return firstClick_; }
        
private:
        virtual DrawObject* createObject() = 0;
        virtual void changeStatusClick() = 0;
        virtual void changeStatusMove() = 0;
        
        DrawObject* cObject_;

        bool firstClick_;
};


template<typename ObjectType>
class TCreateAction : public CreateAction 
{
public:
        TCreateAction<ObjectType>( Controler* parent );

private:
        DrawObject* createObject() { return new ObjectType(); }
        void init();
        void setInitialStatus_private() {}
        void changeStatusClick() {}
        void changeStatusMove() {}
};

template<typename ObjectType>
TCreateAction<ObjectType>::TCreateAction( Controler* parent )
        : CreateAction( parent )
{  
        cursor_ = Qt::UpArrowCursor;
        setText( tr("Create %1").arg( ObjectType::cuteMetaObject().name() ) );
        setIcon( QIcon(":images/" + ObjectType::cuteMetaObject().keyword() + ".png") );
        init();
}


#endif
