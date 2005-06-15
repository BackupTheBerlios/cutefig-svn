 
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

#ifndef compound_h
#define compound_h

#include "drawobject.h"

#include <QList>

typedef QList<DrawObject*> ObjectList;

class Compound : public DrawObject
{
        Q_OBJECT
public:
        Compound( const ObjectList& l, Figure* parent );
        Compound( Compound* c );
        ~Compound() {}

        virtual DrawObject* copy() { return new Compound( this ); }

        virtual const QString objectname() { return "group"; }

        virtual bool pointHits( const QPointF& p, qreal tolerance ) const;
        virtual bool pointHitsOutline( const QPointF& p, qreal tolerance ) const;

        virtual void draw( QPainter* p );
        virtual void drawTentative( QPainter* p );

        virtual QPointF center() const;

        virtual void move( const QPointF& d );
        virtual void mapMatrix( const QMatrix& m );

        virtual void outputToBackend( OutputBackend* ob );

        const ObjectList& childObjects() { return childObjects_; }

protected:
        virtual QPointF* nextPoint() { return 0; }
        virtual void passPointFlag( Fig::PointFlag ) {}
        
        virtual void getReadyForDraw();
        virtual void doSpecificPreparation();

private:
        ObjectList childObjects_;
};

#endif
