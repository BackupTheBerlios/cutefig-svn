 
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


QString percentDecode( const QString& s )
{
        return QUrl::fromPercentEncoding( s.toUtf8() );
}


int get_colorByte( QTextStream& is )
{
        char c;
        is >> c;
        int v = std::toupper(c) - '0';
        if ( v > 9 )
                v -= 7;
        if ( v < 0 || v > 15 )
                is.setStatus( QTextStream::ReadCorruptData );

        return v;
}

int get_colorPart( QTextStream& is )
{
        int v = get_colorByte( is );
        v = v << 4;
        v |= get_colorByte( is );
        return v;
}
        

/*! Tries to interpret the next word of an QTextStream as a
 * QColor. On failure the failbit of the QTextStream is set to true.
 */
QTextStream &operator>> ( QTextStream &is, QColor &color )
{
        is.skipWhiteSpace();
        char c;
        is >> c;
        if ( c != '#' ) {
                is.setStatus( QTextStream::ReadCorruptData );
                return is;
        }
        
        color.setRed( get_colorPart( is ) );
        color.setGreen( get_colorPart( is ) );
        color.setBlue( get_colorPart( is ) );

        if ( !is.atEnd() ) {
                is >> c;
                is.seek( is.pos() - 1 );
                if ( !std::isspace( c ) && c > 0 ) 
                        color.setAlpha( get_colorPart( is ) );
        }
        
        return is;
}

QTextStream& operator>> ( QTextStream& is, QSizeF& size ) 
{
        double w,h;
        is >> w >> h;

        size.setWidth( w );
        size.setHeight( h );

        return is;
}

QTextStream& operator>> ( QTextStream &is, ResourceKey& key )
{
        is.skipWhiteSpace();
        char c;
        is >> c;

        if ( c == '%' ) {
                key = ResourceKey();
                return is;
        }
                
        QString keyString;
        is >> keyString;
        keyString = percentDecode( keyString );
        
        switch ( c ) {
            case '&':
                    key = ResourceKey::builtIn( keyString );
                    break;
            case '*':
                    key = ResourceKey::inFig( keyString );
                    break;
            default:
                    is.setStatus( QTextStream::ReadCorruptData );
                    key = ResourceKey();
        }
        
        return is;
}


QTextStream& operator>>( QTextStream& is, Pen& pen )
{
        double lw;
        ResourceKey dashKey;
        int cs, js;
        
        is >> lw >> dashKey >> cs >> js;

        if ( !is.status() == QTextStream::ReadCorruptData ) {
                pen.setWidth( lw );
                pen.setCapStyle( (Qt::PenCapStyle) cs );
                pen.setJoinStyle( (Qt::PenJoinStyle) js );
                pen.setDashes( dashKey );
        }

        return is;
}

QTextStream& operator>> ( QTextStream &is, Stroke& s )
{
        is.skipWhiteSpace();
        char c;
        is >> c;
        is.seek( is.pos() - 1 );
        
        if ( c == '#' ) {
                QColor color;
                is >> color;
                if ( is.status() == QTextStream::Ok )
                        s = Stroke( color );
        } else {
                ResourceKey key;
                
                s = Stroke();
                is >> key;
                if ( key.isValid() ) {
                
                        QString kw;
                        is >> kw;

			if ( !s.setData( kw, key ) )
                                is.setStatus( QTextStream::ReadCorruptData );
                }
        }

        return is;
}
 

QTextStream& operator>> ( QTextStream &is, QDate& d ) 
{
        QString ds;
        is >> ds;
        d = QDate::fromString( ds, Qt::ISODate );
        if ( !d.isValid() && !d.isNull() )
                is.setStatus( QTextStream::ReadCorruptData );

        return is;
}




//// output

QString percentEncode( const QString& s )
{
	return QUrl::toPercentEncoding( s ).data();
}


void put_colorPart( QTextStream& ts, int v )
{
        int e = v & 0x0F;
        e += (e>9) ? 'A'-10 : '0';
        v = v >> 4;
        v += (v>9) ? 'A'-10 : '0';
        
        ts << (char)v << (char)e;
}

QTextStream& operator<< ( QTextStream& ts, const QColor& c )
{
        ts << '#';
        
        put_colorPart( ts, c.red() );
        put_colorPart( ts, c.green() );
        put_colorPart( ts, c.blue() );
        
        if ( c.alpha() != 0xFF )
                put_colorPart( ts, c.alpha() );
        
        return ts;
}


QTextStream& operator<< ( QTextStream& ts, const QSizeF& s ) 
{
        ts << s.width() << ' ' << s.height();
        return ts;
}


QTextStream& operator<< ( QTextStream& ts, const ResourceKey& key )
{
        if ( !key.isValid() ) {
                ts << '%';
                return ts;
        }
        
        if ( key.isBuiltIn() )
                ts << '&';
        else
                ts << '*';

        ts << percentEncode(key.keyString());

        return ts;
}

QTextStream& operator<< ( QTextStream& ts, const Stroke& st )
{
        if ( st.isHardColor() )
                ts << st.color();
        else
                ts << st.key() << ' ' << st.typeString();

        return ts;
}


QTextStream& operator<< ( QTextStream& os, const QDate& d ) 
{
        os << d.toString( Qt::ISODate );
        return os;
}
