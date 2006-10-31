 
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

#ifndef compound_h
#define compound_h

#include "drawobject.h"
#include "typedefs.h"

#include <QList>

/*! \class Compound
 * 
 *  \brief Implements a Compound object for Group/Ungroup
 *  functionality.
 */

class Compound : public DrawObject
{
        Q_OBJECT
        DRAW_OBJECT( "group", "" )
public:
        Compound( const ObjectList& l, const Figure* fig );
        Compound( const Compound* c );

        ~Compound();

        void releaseChildren();

        virtual bool pointHits( const QPointF& p, qreal tolerance ) const;
        virtual bool pointHitsOutline( const QPointF& p, qreal tolerance ) const;

        virtual void draw( QPainter* p ) const;
        virtual void drawTentative( QPainter* p ) const;

        virtual QPointF center() const;

        virtual void move( const QPointF& d );
        virtual void mapMatrix( const QMatrix& m );

        virtual const ResourceSet usedResources() const;
        virtual void releaseResources();
        virtual void reclaimResources();

        virtual void outputToBackend( OutputBackend* ob ) const;

        const ObjectList& childObjects() const { return childObjects_; }

        const Compound* compoundParent() const { return compoundParent_; }
        
protected:
        virtual int nextPointIndex() { return -1; }
        virtual void passPointFlag( Fig::PointFlags ) {}
        
        virtual void getReadyForDraw();
        virtual void doSpecificPreparation();
        
        virtual void setupRects() {}
        virtual void setupPainterPath() {}
        
private:
        ObjectList childObjects_;
};

#endif
