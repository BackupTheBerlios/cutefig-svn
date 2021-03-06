 
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

class Ellipse : public Rectangloid
{
        Q_OBJECT
        DRAW_OBJECT( "ellipse", "&Ellipse" );
        
public:
        explicit Ellipse();
        Ellipse( const Ellipse *o );

        virtual bool pointHitsOutline( const QPointF& p, qreal tolerance ) const;

        bool isCircle() const { return circle_; }
        bool isSpecifiedByRadii() const { return specByRadii_; }

        void setSpecifiedByRadii( bool r );
        void setCircle( bool c );

        virtual void outputToBackend( OutputBackend* ob ) const;
        

private:
        bool specByRadii_, circle_;


	//! The two focal points are needed for internal stuff in pointHitsOutline().
        QPointF fp1_, fp2_;

	//! the ellipse radius
        double rad_d_;
        
        void passPointFlag_private( Fig::PointFlags f );
        
        void doSpecificPreparation();
        void setupWidthAndHeight();
        void addPath( const QRectF& r );
};


#endif
