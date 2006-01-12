 
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


#include "gradient.h"

#include <cmath>

#include <QHash>

#include <QDebug>

Gradient::Gradient( Type type, const QPointF& start, const QPointF& final )
        : type_( type ),
          startPoint_( start ),
          finalPoint_( final ),
          colorStops_(),
          radius_( .5 )
{ 
}

// Gradient::Gradient( const Gradient& other )
//         : type_( other.type_ ),
//           startPoint_( other.startPoint_ ),
//           finalPoint_( other.finalPoint_ ),
//           colorStops_( other.colorStops_ ),
//           radius_( other.radius_ ) 
// {
// }

// Gradient& Gradient::operator= ( const Gradient& other )
// {
//         type_ = other.type_;
//         startPoint_ = other.startPoint_;
//         finalPoint_ = other.finalPoint_;
//         colorStops_ = other.colorStops_;
//         radius_ = other.radius_;

//         return *this;
// }


void ensureRange( qreal& val )
{
        if( val > 1. )
                val = 1.;
        else if ( val < 0 )
                val = 0;
}

void ensureRange( QPointF& point )
{
        ensureRange( point.rx() );
        ensureRange( point.ry() );
}


void Gradient::setStartPoint( const QPointF& p )
{
        startPoint_ = p;
        ensureRange( startPoint_ );
}

void Gradient::setFinalPoint( const QPointF& p )
{
        finalPoint_ = p;
        ensureRange( finalPoint_ );
}

void Gradient::setColorAt( qreal pos, const QColor& c )
{
        colorStops_ << qMakePair( pos, c );
}


QGradient* Gradient::toQGradient( const QRectF& rect ) const
{
        QGradient* gr = 0;
        
        switch ( type_ ) {
            case Linear:
                    gr = new QLinearGradient(rect.left() + startPoint_.x()*rect.width(),
                                             rect.top()  + startPoint_.y()*rect.height(),
                                             rect.left() + finalPoint_.x()*rect.width(),
                                             rect.top()  + finalPoint_.y()*rect.height());
                    break;
            case Radial: {
                    qreal rad = hypot( rect.width(), rect.height() );
                    rad *= radius_;
                    gr = new QRadialGradient(rect.left() + startPoint_.x()*rect.width(),
                                             rect.top()  + startPoint_.y()*rect.height(),
                                             rad,
                                             rect.left() + finalPoint_.x()*rect.width(),
                                             rect.top()  + finalPoint_.y()*rect.height() );
            }
                    break;
            default: break;
        }

        if ( gr )
                gr->setStops( colorStops_ );

        return gr;
}

unsigned int qHash( const Gradient& g )
{
        QByteArray data;
        QDataStream ds( &data, QIODevice::WriteOnly );

        ds << g.type() << g.radius() << g.startPoint() << g.finalPoint() << g.colorStops();

        return qHash( data );
}


#include "reslib.h"

template<>
void ResLib<Gradient>::init()
{
        Gradient defaultGrad( Gradient::Linear, QPointF(0,0), QPointF(1,1) );
        Gradient linGrad( Gradient::Linear, QPointF(0,0), QPointF(1,1) );
        Gradient radGrad( Gradient::Radial, QPointF(.5,.5), QPointF(.75,.25) ) ;
        radGrad.setRadius( .3 );
        
        defaultGrad.setColorAt( 0, Qt::black );
        defaultGrad.setColorAt( 1, Qt::white );
        
        linGrad.setColorAt( 0, Qt::red );
        linGrad.setColorAt( 1, Qt::blue );

        radGrad.setColorAt( 0, Qt::red );
        radGrad.setColorAt( 1, Qt::blue );
        
        insertBuiltIn( "defaultGradient", defaultGrad );
        insertBuiltIn( "linearBuiltIn",   linGrad );
        insertBuiltIn( "radialBuiltIn",   radGrad );
}


QDebug operator<<(QDebug dbg, const Gradient& g)
{
        dbg.nospace() << "Gradient: ";
        
        switch ( g.type() ) {
            case Gradient::None: dbg.nospace() << "None "; break;
            case Gradient::Linear: dbg.nospace() << "Linear "; break;
            case Gradient::Radial: dbg.nospace() << "Radial "; break;
            default: break;
        }

        dbg.nospace() << g.startPoint() << g.finalPoint();

        return dbg.space();
}


