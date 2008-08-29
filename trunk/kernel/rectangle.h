 
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

#ifndef rectangle_h
#define rectangle_h

#include "rectangloid.h"

template<typename ObjectName> class TObjectHandler;

class Rectangle : public Rectangloid
{
        Q_OBJECT
        DRAW_OBJECT( "rectangle", "&Rectangle" );
public:
        explicit Rectangle();
        Rectangle( const Rectangle* r );

        double xCornerRad() const;
        void setXCornerRad( double r );

        double yCornerRad() const;
        void setYCornerRad( double r );

        bool hasRoundedCorners() const;
        bool hasEqualCornerRadii() const;

        void setRoundedCorners( bool rc );
        void setEqualRoundedCorners( bool eq );

	double maxXCornerRad() const;
	double maxYCornerRad() const;
        
        virtual bool pointHitsOutline( const QPointF& p, qreal tolerance ) const;

        virtual void outputToBackend( OutputBackend* ob ) const;

        friend class TObjectHandler<Rectangle>;
        
private:
        virtual void passPointFlag_private( Fig::PointFlags ) {}
        virtual void setupWidthAndHeight();
        virtual void addPath( const QRectF& r );

        double xCornerRad_;
        double yCornerRad_;

	bool roundedCorners_;
	bool equalCornerRadii_;
};

#endif
