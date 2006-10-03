 
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

#ifndef rectagnloid_h
#define rectagnloid_h

#include "drawobject.h"
#include "geometry.h"

//#include <cmath>

class Rectangloid : public DrawObject 
{
        Q_OBJECT
public:
        explicit Rectangloid( Figure* parent = 0 );
        Rectangloid( const Rectangloid* r );

        virtual bool canHaveArrows() const { return false; }
        
        double angle() const { return angle_ * Geom::rad; }
        QPointF center() const { return center_; }

        virtual void drawMetaData( QPainter* p ) const;
        

//        virtual void drawTentative( QPainter* p ) const;
        
        void setAngleNew( double a );

public slots:
        void setAngle( double a );
        void setAngle( int a );

protected:
	double angleRad() const { return angle_; }

        void setupPainterPath();
        void setupRects();
        int nextPointIndex();
        
        void passPointFlag( Fig::PointFlags f );
        virtual void passPointFlag_private( Fig::PointFlags f ) = 0;

        virtual void setupWidthAndHeight() = 0;
        virtual void addPath() = 0;

        QPointF center_;
        qreal w_, h_, w2_, h2_;

//        static const double rad = 180/M_PI;
        
private:
	double angle_;
        
        QMatrix rotation_;

        bool hasAngle_;
	QRectF oldRect_;

        void calcAngle();


};


#endif
