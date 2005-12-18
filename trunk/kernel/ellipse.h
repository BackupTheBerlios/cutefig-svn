 
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

#ifndef ellipse_h
#define ellipse_h

#include <cmath>

#include <QMatrix>

#include "rectangloid.h"

CUTE_DECLARE_DRAWOBJECT( Ellipse, "ellipse", "&Ellipse" );
        
class Ellipse : public Rectangloid
{
        Q_OBJECT
        DRAW_OBJECT( Ellipse );
        
public:
        Ellipse( Figure* parent=0 );
        Ellipse( const Ellipse *o );
        ~Ellipse() {}

//        virtual DrawObject* copy();

        virtual bool pointHitsOutline( const QPointF& p, qreal tolerance ) const;

        bool isCircle() const { return circle_; }
        bool isSpecifiedByRadii() const { return specByRadii_; }

        virtual void outputToBackend( OutputBackend* ob );
        

public slots:
        void setSpecByRadii( bool r );
        void setIsCircle( bool c );

private:
        bool specByRadii_, circle_;

        QPointF fp1_, fp2_;
        qreal rad_d_;
        
        void passPointFlag_private( Fig::PointFlag f );
        
        void doSpecificPreparation();
        void setupWidthAndHeight();
        void addPath();
};


#endif
