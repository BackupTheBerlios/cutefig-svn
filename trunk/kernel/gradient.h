 
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

class GradientHandler;

class Gradient
{
public:
        friend class GradientHandler;
        enum Type { None=0, Linear, Radial };

        Gradient() : colorStops_() {}
        virtual ~Gradient() {}

        Gradient( const Gradient* other ) : colorStops_( other->colorStops_ ) {}
        virtual Gradient* copy() const { return 0; }
        
        QGradientStops& colorStops() { return colorStops_; }
        void setColorStops( const QGradientStops& stops ) { colorStops_ = stops; }
        void setColorAt( qreal pos, const QColor& c );

        virtual QPointF firstPoint() const = 0;
        virtual QPointF secondPoint() const = 0;
        
        virtual QGradient* toQGradient( const QRectF& ) const { return 0; }
        virtual Type type() const { return None; }
        
protected:
        void ensureRange( QPointF& point );
        void ensureRange( qreal& val );

        QGradientStops colorStops_;

private:
        virtual QPointF* first() = 0;
        virtual QPointF* second() = 0;
};
        
class LinearGradient : public Gradient
{
public:
        LinearGradient( const QPointF& start, const QPointF& final )
                : Gradient(), start_( start ), final_( final ) {}

        LinearGradient( const LinearGradient* other )
                : Gradient( other ),
                  start_( other->start_ ),
                  final_( other->final_ ) {}

        ~LinearGradient() {}
        
        virtual Gradient* copy() const { return new LinearGradient( this ); }
        
        void setStartPoint( const QPointF& p );
        void setFinalPoint( const QPointF& p );

        QPointF startPoint() const { return start_; }
        QPointF finalPoint() const { return final_; }

        QPointF firstPoint() const { return start_; }
        QPointF secondPoint() const { return final_; }

        virtual QGradient* toQGradient( const QRectF& rect ) const;
        virtual Type type() const { return Linear; }
        
private:
        QPointF start_, final_;

        virtual QPointF* first() { return &start_; }
        virtual QPointF* second() { return &final_; }
};

class RadialGradient : public Gradient
{
public:
        RadialGradient( const QPointF& center, const QPointF& focal, qreal radius )
                : Gradient(), center_( center ), focal_( focal ), radius_( radius ) {}

        RadialGradient( const RadialGradient* other )
                : Gradient( other ),
                  center_( other->center_ ),
                  focal_( other->focal_ ),
                  radius_( other->radius_ ) {}

        ~RadialGradient() {}
        
        virtual Gradient* copy() const { return new RadialGradient( this ); }
        
        void setCenterPoint( const QPointF& p );
        void setFocalPoint( const QPointF& p );
        void setRadius( qreal r );

        QPointF centerPoint() const { return center_; }
        QPointF focalPoint() const { return focal_; }

        QPointF firstPoint() const { return center_; }
        QPointF secondPoint() const { return focal_; }

        qreal radius() const { return radius_; }
        
        virtual QGradient* toQGradient( const QRectF& rect ) const;
        virtual Type type() const { return Radial; }

private:
        QPointF center_, focal_;
        qreal radius_;

        virtual QPointF* first() { return &center_; }
        virtual QPointF* second() { return &focal_; }
};


class GradientHandler
{
public:
        GradientHandler( Gradient* gr ) : gradient_( gr ) {}

        QPointF* first() { return gradient_->first(); }
        QPointF* second() { return gradient_->second(); }

private:
        Gradient* gradient_;
};

#endif
