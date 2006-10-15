 
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

#include "streamops.h"
#include "gradient.h"
#include "pixmap.h"
#include "resourceio.h"
#include "reslib.h"
#include "typedefs.h"

#include <QColor>
#include <istream>

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
void TResourceIO<QColor>::outputResourceBody( const QColor& res, std::ostream& stream ) const 
{
        stream << res << " ";
}



// Gradient

void createGradient( std::istream& is, Gradient& gradient ) 
{
        QString type;
        double x1,x2, y1,y2;

        is >> type >> x1 >> y1 >> x2 >> y2;

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
                        errorString_ = tr("gradstop expected");
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
void TResourceIO<Gradient>::outputResourceBody( const Gradient& res, std::ostream& ts ) const
{
        const QGradientStops& stops = res.colorStops();
        
        if ( res.type() == Gradient::Linear ) 
                ts << "linear ";
        else 
                ts << "radial ";

        ts << res.startPoint().x() << ' ' << res.startPoint().y()  << ' '
           << res.finalPoint().x() << ' ' << res.finalPoint().y();
        if ( res.type() == Gradient::Radial )
                ts << res.radius();

        ts << "\n";

        foreach ( QGradientStop s, stops )
                ts << "gradstop " << s.first << ' ' << s.second << "\n";        
}



// Pixmaps

template<>
bool TResourceIO<Pixmap>::parseResource( const QString& itemtype, std::istream& is )
{
	QString fileName;
	is >> fileName;

	if ( !resource_.loadFromFile( fileName ) ) {
		errorString_ = tr("Could not read pixmap file %1.").arg( fileName );
		failed_ = true;
	}

	return true;
}


template<>
void TResourceIO<Pixmap>::outputResourceBody( const Pixmap& res, std::ostream& ts ) const
{
	if ( res.isFromFile() ) {
		ts << res.fileName() << "\n";
		return;
	}
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
void TResourceIO<Dashes>::outputResourceBody( const Dashes& res, std::ostream& ts ) const
{
        foreach ( double d, res )
                ts << d << ' ';
}

