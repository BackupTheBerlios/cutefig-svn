 
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

CUTE_DECLARE_DRAWOBJECT( Compound, "group", "" );

/** \class Compound
 * 
 *  \brief Implements a Compound object for Group/Ungroup
 *  functionality.
 */

class Compound : public DrawObject
{
        Q_OBJECT
        DRAW_OBJECT( Compound )
public:
        Compound( const ObjectList& l, Figure* parent );
        Compound( const Compound* c );
        ~Compound() {}

        void releaseChildren();

        virtual bool pointHits( const QPointF& p, qreal tolerance ) const;
        virtual bool pointHitsOutline( const QPointF& p, qreal tolerance ) const;

        virtual void draw( QPainter* p );
        virtual void drawTentative( QPainter* p );

        virtual QPointF center() const;

        virtual void move( const QPointF& d );
        virtual void mapMatrix( const QMatrix& m );

        virtual const ResourceSet usedResources() const;
        virtual void releaseResources();
        virtual void reclaimResources();

        virtual void outputToBackend( OutputBackend* ob );

        const ObjectList& childObjects() const { return childObjects_; }

protected:
        virtual QPointF* nextPoint() { return 0; }
        virtual void passPointFlag( Fig::PointFlag ) {}
        
        virtual void getReadyForDraw();
        virtual void doSpecificPreparation();
        
        virtual void setupRects() {}
        virtual void setupPainterPath() {}
        
private:
        ObjectList childObjects_;
};

#endif
