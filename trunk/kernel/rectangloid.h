 
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

#include <cmath>

class Rectangloid : public DrawObject 
{
        Q_OBJECT
public:
        explicit Rectangloid( Figure* parent = 0 );
        Rectangloid( const Rectangloid* r );

        double angle() const { return angle_ * rad; }
        QPointF center() const { return center_; }

        virtual void drawTentative( QPainter* p, const QPen& auxpen ) const;
        
        void setAngleNew( double a );

public slots:
        void setAngle( double a );
        void setAngle( int a );

protected:
        double angle_;
        
        QPointF center_;
        qreal w_, h_, w2_, h2_;

        QMatrix rotation_;

        QRectF oldRect_;

        void setupPainterPath();
        void setupRects();
        int nextPointIndex();
        
        void passPointFlag( Fig::PointFlag f );
        virtual void passPointFlag_private( Fig::PointFlag f ) = 0;

        virtual void setupWidthAndHeight() = 0;
        virtual void addPath() = 0;
//        virtual drawTentative_private( QPainter* p ) const = 0;
        
        static const double rad = 180/M_PI;
        
private:
        bool hasAngle_;
        void calcAngle();


};


#endif
