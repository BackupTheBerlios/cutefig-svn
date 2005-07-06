 
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

#ifndef gradient_h
#define gradient_h

#include <QBrush>
#include <QSizeF>
#include <QRectF>
#include <QMetaType>

class Gradient
{
public:
        enum Type { None=0, Linear, Radial };

        Gradient() : colorStops_() {}
        virtual ~Gradient() {}

        Gradient( Gradient* other ) : colorStops_( other->colorStops_ ) {}
        virtual Gradient* copy() { return 0; }
        
        QGradientStops& colorStops() { return colorStops_; }
        void setColorAt( qreal pos, const QColor& c );
        
        virtual QGradient* toQGradient( const QRectF& ) const { return 0; }
        virtual Type type() const { return None; }
        
protected:
        void ensureRange( QPointF& point );
        void ensureRange( qreal& val );

        QGradientStops colorStops_;
};

Q_DECLARE_METATYPE( Gradient )


        
class LinearGradient : public Gradient
{
public:
        LinearGradient( const QPointF& start, const QPointF& final )
                : Gradient(), start_( start ), final_( final ) {}

        LinearGradient( LinearGradient* other )
                : Gradient( other ),
                  start_( other->start_ ),
                  final_( other->final_ ) {}

        virtual Gradient* copy() { return new LinearGradient( this ); }
        
        void setStartPoint( const QPointF& p );
        void setFinalPoint( const QPointF& p );

        QPointF startPoint() const { return start_; }
        QPointF finalPoint() const { return final_; }

        virtual QGradient* toQGradient( const QRectF& rect ) const;
        virtual Type type() const { return Linear; }
        
private:
        QPointF start_, final_;
};

class RadialGradient : public Gradient
{
public:
        RadialGradient( const QPointF& center, const QPointF& focal, qreal radius )
                : Gradient(), center_( center ), focal_( focal ), radius_( radius ) {}

        RadialGradient( RadialGradient* other )
                : Gradient( other ),
                  center_( other->center_ ),
                  focal_( other->focal_ ),
                  radius_( other->radius_ ) {}

        virtual Gradient* copy() { return new RadialGradient( this ); }
        
        void setCenterPoint( const QPointF& p );
        void setFocalPoint( const QPointF& p );
        void setRadius( qreal r );

        QPointF centerPoint() const { return center_; }
        QPointF focalPoint() const { return focal_; }
        qreal radius() const { return radius_; }
        
        virtual QGradient* toQGradient( const QRectF& rect ) const;
        virtual Type type() const { return Radial; }

private:
        QPointF center_, focal_;
        qreal radius_;
};

#endif
