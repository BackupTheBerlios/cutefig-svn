 
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

#include "arrow.h"
#include "reslib.h"

#include <QPainter>


class StandardArrowPainter : public ArrowPainter 
{
public:
        virtual void draw( const QPointF& pos, const QPointF& angle, QPainter* p,
                           double w, double l ) const;
};
        
void StandardArrowPainter::draw( const QPointF& pos, const QPointF& angle, QPainter* p,
                                 double w, double l ) const 
{
        if ( angle.isNull() )
                return;

        QBrush b = p->brush();

        QPen pen = p->pen();
        pen.setJoinStyle( Qt::MiterJoin );
        p->setPen( pen );
        p->setBrush( p->pen().brush() );

        QPointF hp = pos - l * pen.width() * angle;
        QPointF d = ( w * pen.width() ) / 2 * QPointF( angle.y(), -angle.x() );

        QPolygonF pol;
        pol << pos << hp + d << hp - d;
        p->drawPolygon( pol );

        p->setBrush( b );
}


class DotEndArrowPainter : public ArrowPainter
{
public:
        virtual void draw( const QPointF& pos, const QPointF& angle, QPainter* p,
                           double w, double l ) const;
};

 void DotEndArrowPainter::draw( const QPointF& pos, const QPointF&, QPainter* p,
                               double w, double ) const
{
        QPen pen( p->pen() );
        w *= pen.width();
        double r = w/2;
        QRectF rect( pos.x()-r, pos.y()-r, w, w );

        QBrush b = p->brush();
        p->setBrush( pen.brush() );
        p->drawEllipse( rect );
        p->setBrush( b );
}


template<>
void ResLib<ArrowPainter*>::init()
{
///        Arrow null;
//        Arrow std( new StandardArrowPainter() );

//        insertBuiltIn( null.type(), null );
        insertBuiltIn( "STD", new StandardArrowPainter() );
        insertBuiltIn( "dot", new DotEndArrowPainter() );
}

template<>
ResourceKey ResLib<ArrowPainter*>::defaultKey()
{
        return ResourceKey::builtIn("STD");
}



Arrow::Arrow( ArrowPainter* ap, double l, double w )
        : arrowPainter_( ap ),
          width_( w ),
          length_( l )
{    
}

Arrow::Arrow( const ResourceKey& k, double w, double l )
        : arrowPainter_(),
          width_( w ),
          length_( l )
{
        arrowPainter_.setResource( k );
}

Arrow& Arrow::operator= ( const Arrow& other )
{
        width_ = other.width_;
        length_ = other.length_;
        
        arrowPainter_ = other.arrowPainter_;

        return *this;
}

void Arrow::draw( const QPointF& pos, const QPointF& angle, QPainter* p ) const
{
        if ( arrowPainter_.data() )
                arrowPainter_.data()->draw( pos, angle, p, width_, length_ );
}


bool Arrow::isValid() const
{
        return arrowPainter_.data();
}

        


// uint qHash( const Arrow& a ) 
// {
//         return qHash( a.type() ) ^
//                 qHash( uint( a.width()*100 ) ) ^ qHash( uint( a.length()*100 ) );
// }

