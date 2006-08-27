 
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

#ifndef arc_h
#define arc_h

#include "drawobject.h"

class Arc : public DrawObject
{
        Q_OBJECT
        DRAW_OBJECT( "arc", "&Arc" );
                
public:
        explicit Arc( Figure* parent );
        Arc( const Arc* other );

        virtual int minPoints() const { return 2; }

        virtual void outputToBackend( OutputBackend* ob ) const;

        void drawMetaData( QPainter* p ) const;
        QSizeF rectSize() const { return rectSize_; }
        void setRectSize( const QSizeF& s );

        double angle() const { return angle_; }
        void setAngle( double a );

        bool arcFlag() const { return arcFlag_; }
        void setArcFlag( bool af ) { arcFlag_ = af; }

        bool sweepFlag() const { return sweepFlag_; }
        void setSweepFlag( bool sf ) { sweepFlag_ = sf; }
        
        
protected:
        virtual void setupPainterPath();
        virtual void setupRects();

//        virtual void getReadyForDraw();

        virtual void doSpecificPreparation();
        
        virtual void passPointFlag( Fig::PointFlags f );
        virtual int nextPointIndex();

//        virtual bool pointHitsOutline( const QPointF& p, qreal tolerance ) const;
        

//        virtual QPointF startAngle();
//        virtual QPointF endAngle();
        
private:
        bool isCircle_;

        // common parameters
        QSizeF rectSize_;

        
        // endpoint parameters
        double angle_;
        bool arcFlag_, sweepFlag_;
        

        // center parameters
        QPointF center_;
        double startAngle_, sweepLength_;
        int direction_;


        QPointF arrowAngle( int angle );

        void calcCenterParameters();
        void calcEndPointParameters();
        
        QMatrix rotation_;
//         void calcDirection( int& startAngle, int& sweepLength, int& direction ) const;
//         void calcAngles( int& startAngle, int& sweepLength ) const;
};


#endif
