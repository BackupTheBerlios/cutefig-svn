 
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

#include "resourcekey.h"
#include "figure.h"
#include "pen.h"
#include "stroke.h"

#include <QString>
#include <QColor>
#include <QUrl>
#include <QSizeF>

#include <iostream>
#include <sstream>

/** Puts the next word of the a std::istream into a QString. 
 *
 */
std::istream &operator>> ( std::istream &is, QString &_s ) 
{
        std::string s;
        is >> s;
        _s = QUrl::fromPercentEncoding( s.c_str() );
        
        return is;
}

int get_colorByte( std::istream& is )
{
         int v = toupper(is.get()) - '0';
         if ( v > 9 )
                 v -= 7;
         if ( v < 0 || v > 15 )
                 is.setstate( is.rdstate() | std::istream::failbit );
         return v;
}

int get_colorPart( std::istream& is )
{
        int v = get_colorByte( is );
        v = v << 4;
        v |= get_colorByte( is );
        return v;
}
        

/** Tries to interpret the next word of an std::istream as a
 * QColor. On failure the failbit of the std::istream is set to true.
 */
std::istream &operator>> ( std::istream &is, QColor &color )
{
        char c;
        do 
                c = is.get();
        while( isspace( c ) );
        
        if ( c != '#' ) {
                is.setstate( is.rdstate() | std::istream::failbit );
                return is;
        }
        
        color.setRed( get_colorPart( is ) );
        color.setGreen( get_colorPart( is ) );
        color.setBlue( get_colorPart( is ) );

        c = is.peek();
        if ( !is.eof() ) {
                if ( !isspace( c ) && c > 0 ) 
                        color.setAlpha( get_colorPart( is ) );
        }
        
        return is;
}

std::istream& operator>> ( std::istream& is, QSizeF& size ) 
{
        double w,h;
        is >> w >> h;

        size.setWidth( w );
        size.setHeight( h );

        return is;
}

std::istream& operator>> ( std::istream &is, ResourceKey& key )
{
        char c;
        do
                c = is.get();
        while ( isspace(c) );

        if ( c == '%' ) {
                key = ResourceKey();
                return is;
        }
                
        QString keyString;
        is >> keyString;
        
        switch ( c ) {
            case '&':
                    key = ResourceKey::builtIn( keyString );
                    break;
            case '*':
                    key = ResourceKey::inFig( keyString );
                    break;
            default:
                    is.setstate( is.rdstate() | std::istream::failbit );
                    key = ResourceKey();
        }
        
        return is;
}


std::istream& operator>>( std::istream& is, Pen& pen )
{
        double lw;
        ResourceKey dashKey;
        int cs, js;
        
        is >> lw >> dashKey >> cs >> js;

        if ( !is.fail() ) {
                pen.setWidth( lw );
                pen.setCapStyle( (Qt::PenCapStyle) cs );
                pen.setJoinStyle( (Qt::PenJoinStyle) js );
                pen.setDashes( dashKey );
        }

        return is;
}

std::istream& operator>> ( std::istream &is, Stroke& s )
{
        char c;
        do 
                c = is.get();
        while ( isspace(c) );

        is.putback( c );
        
        if ( c == '#' ) {
                QColor color;
                if (is >> color)
                        s = Stroke( color );
        } else {
                ResourceKey key;
                
                s = Stroke();
                if ( (is >> key) && key.isValid() ) {
                
                        QString kw;
                        is >> kw;
                        
                        if ( kw == Res::resourceName<QColor>() ) 
                                s.setColor( key );
                        else if ( kw == Res::resourceName<Gradient>() )
                                s.setGradient( key );
                }
        }

        return is;
}
 

std::istream& operator>> ( std::istream &is, QDate& d ) 
{
        QString ds;
        is >> ds;
        d = QDate::fromString( ds, Qt::ISODate );
        if ( !d.isValid() && !d.isNull() )
                is.setstate( is.rdstate() | std::istream::failbit );

        return is;
}




//// output

std::ostream& operator<< ( std::ostream& ts, const QString& s )
{
        ts << QUrl::toPercentEncoding( s ).data();

        return ts;
}


void put_colorPart( std::ostream& ts, int v )
{
        int e = v & 0x0F;
        e += (e>9) ? 'A'-10 : '0';
        v = v >> 4;
        v += (v>9) ? 'A'-10 : '0';
        
        ts << (char)v << (char)e;
}

std::ostream& operator<< ( std::ostream& ts, const QColor& c )
{
        ts << '#';
        
        put_colorPart( ts, c.red() );
        put_colorPart( ts, c.green() );
        put_colorPart( ts, c.blue() );
        
        if ( c.alpha() != 0xFF )
                put_colorPart( ts, c.alpha() );
        
        return ts;
}


std::ostream& operator<< ( std::ostream& ts, const QSizeF& s ) 
{
        ts << s.width() << ' ' << s.height();
        return ts;
}


std::ostream& operator<< ( std::ostream& ts, const ResourceKey& key )
{
        if ( !key.isValid() ) {
                ts << '%';
                return ts;
        }
        
        if ( key.isBuiltIn() )
                ts << '&';
        else
                ts << '*';

        ts << key.keyString();

        return ts;
}

std::ostream& operator<< ( std::ostream& ts, const Stroke& st )
{
        if ( st.isHardColor() )
                ts << st.color();
        else
                ts << st.key() << ' ' << st.typeString();

        return ts;
}


std::ostream& operator<< ( std::ostream& os, const QDate& d ) 
{
        os << d.toString( Qt::ISODate );
        return os;
}
