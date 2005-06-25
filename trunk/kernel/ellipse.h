 
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

#include "drawobject.h"

class Ellipse : public DrawObject
{
        Q_OBJECT
public:
        Ellipse( Figure* parent=0 );
        Ellipse( Ellipse *o );
        ~Ellipse() {};

        virtual DrawObject* copy();

        virtual const QString objectname() { return "ellipse"; };

        virtual bool pointHitsOutline( const QPointF& p, qreal tolerance ) const;

        double angle() const { return angle_ * rad; };
        QPointF center() const { return center_; };

        bool isCircle() const { return circle_; };
        bool isSpecifiedByRadii() const { return specByRadii_; };

        virtual void outputToBackend( OutputBackend* ob );
        
        void setAngleNew( double a );

public slots:
        void setSpecByRadii( bool r );
        void setIsCircle( bool c );
        void setAngle( double a );
        void setAngle( int a );


private:
        bool specByRadii_, circle_;

        double angle_;
        QPointF center_, fp1_, fp2_;
        qreal w_, h_, w2_, h2_, rad_d_;

        QMatrix rotation_;

        QRectF oldRect_;
        void setupPainterPath();
        void setupRects();
        void doSpecificPreparation();
        QPointF* nextPoint();
        
        void passPointFlag( Fig::PointFlag f );

        static const double rad = 180/M_PI;
};


#endif
