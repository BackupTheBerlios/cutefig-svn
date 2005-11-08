 
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

#include "stdresio.h"
#include "streamops.h"
#include "gradient.h"

#include <QDebug>


// QColor

template<>
bool TResourceIO<QColor>::parseResource( const QString&, std::istream& is )
{
        is >> resource_;
        
        failed_ = is.fail();

        return false;
}

template<>
void TResourceIO<QColor>::outputResourceBody( std::ostream& stream)
{
        stream << resource_ << " ";
}



// Gradient

void createGradient( std::istream& is, Gradient& gradient ) 
{
        QString type;
        QColor startColor, endColor;
        double x1,x2, y1,y2;

        is >> type >> startColor >> endColor >> x1 >> y1 >> x2 >> y2;

        QPointF startPoint( x1,y1 );
        QPointF endPoint( x2,y2 );
        
        if ( type == "radial" ) {
                double rad;
                is >> rad;
                gradient = Gradient( Gradient::Radial, startPoint, endPoint );
                gradient.setRadius( rad );
        }
        else 
                gradient = Gradient( Gradient::Linear, startPoint, endPoint );

        gradient.setColorAt( 0.0, startColor );
        gradient.setColorAt( 1.0, endColor );
}


template<>
bool TResourceIO<Gradient>::parseResource( const QString& itemtype, std::istream& is ) 
{
        if ( itemtype.isNull() ) {
                createGradient( is, resource_ );
                if ( is.fail() ) {
                        errorString_ = tr("Invalid gradient line");
                        failed_ = true;
                }
        } else {
                if ( itemtype != "gradstop" ) {
                        errorString_ = tr("gradstop expeted");
                        failed_ = true;
                        return false;
                }

                double pos;
                QColor cl;
                
                is >> pos >> cl;
                if ( is.fail() ) {
                        errorString_ = tr("invalid gradstop line");
                        failed_ = true;
                } else
                        resource_.setColorAt( pos, cl );
        }

        return true;
}

template<>
void TResourceIO<Gradient>::postProcessResource() 
{
        QGradientStops& stops = resource_.colorStops();
        
        if ( stops.size() > 2 ) {
                QGradientStop h = stops[1];
                stops[1] = stops.last();
                stops.last() = h;
        }
}

                
template<>
void TResourceIO<Gradient>::outputResourceBody( std::ostream& ts )
{
        const QGradientStops& stops = resource_.colorStops();
        
        if ( resource_.type() == Gradient::Linear ) 
                ts << "linear ";
        else 
                ts << "radial ";

        ts << stops.first().second << ' ' << stops.last().second << ' ';

        ts << resource_.startPoint().x() << ' ' << resource_.startPoint().y()  << ' '
           << resource_.finalPoint().x() << ' ' << resource_.finalPoint().y();
        if ( resource_.type() == Gradient::Radial )
                ts << resource_.radius();

        ts << "\n";

        for ( int i = 1; i < stops.size()-1; i++ )
                ts << "gradstop " << stops[i].first << ' ' << stops[i].second << "\n";
}

        
// Dashes

template<>
bool TResourceIO<Dashes>::parseResource( const QString&, std::istream& is ) 
{
        double val;

        while ( is >> val && val > 0 )
                resource_.push_back( val );        

        return false;
}

template<>
void TResourceIO<Dashes>::outputResourceBody( std::ostream& ts )
{
        foreach ( double d, resource_ )
                ts << d << ' ';
}

