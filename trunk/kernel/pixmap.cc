 
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


#include "pixmap.h"
#include "reslib.h"

#include <QImageReader>
#include <QImageWriter>
#include <QByteArray>
#include <QBuffer>
#include <QDataStream>
#include <QHash>


Pixmap::Pixmap()
	: image_(),
          format_(),
          fileName_()
{
}

void Pixmap::setImage( const QImage& img )
{
	fileName_.clear();
	image_ = img;
}

const QString Pixmap::loadFromFile( const QString& filename )
{
        qDebug() << __PRETTY_FUNCTION__ << filename;
        QImageReader ir( filename );
        format_ = ir.format();

        image_ = ir.read();
        if ( image_.isNull() ) {
                format_.clear();
                qDebug() << ir.errorString();
                return ir.errorString();
        }

	fileName_ = filename;
        qDebug() << filename << format_;

	return QString();
}

bool Pixmap::isFromFile() const
{
	return !fileName_.isEmpty();
}






template<> void ResLib<Pixmap>::init()
{
        insertBuiltIn("nopixmap", Pixmap());
}

template<> ResourceKey ResLib<Pixmap>::defaultKey()
{
        return ResourceKey::builtIn("nopixmap");
}

template<>
const QString ResLib<Pixmap>::resourceName()
{
        return "pixmap";
}


template<>
bool TResourceIO<Pixmap>::parseResource( const QString& itemtype, QTextStream& is )
{
        if ( itemtype.isEmpty() ) {
                QByteArray fmt;
                is >> fmt;
                resource_.setFormat( fmt );
        } else
                streamToBuffer( is );

	return true;
}

template<>
void TResourceIO<Pixmap>::postProcessResource() 
{
        QTextStream ts( &buffer(), QIODevice::ReadOnly );
        const QByteArray imgdata = QByteArray::fromBase64( buffer().toAscii() );
        QBuffer buf;
        buf.setData( imgdata );
        buf.open( QIODevice::ReadOnly );
        QImageReader ir( &buf, resource_.format() );

        QImage img = ir.read();
        resource_.setImage( img );

        if ( img.isNull() ) {
                errorString_ = ir.errorString();
                failed_ = true;
        }
}


template<>
void TResourceIO<Pixmap>::outputResourceBody( const Pixmap& res, QTextStream& ts ) const
{
        QByteArray fmt = res.format();

        QBuffer buf;
        QImageWriter iw( &buf, fmt );
        iw.write( res.image() );
        
        ts << res.format() << "\n";

        const QByteArray base64 = buf.data().toBase64();
        for ( int pos=0; pos < base64.size(); pos += 76 )
                ts << base64.mid( pos,76 ) << "\n";
}

static TResourceIOFactory<Pixmap> rIOFinstance;


unsigned int qHash( const Pixmap& pm )
{
        QByteArray ba;
	QDataStream st( &ba, QIODevice::WriteOnly );

	st << pm.image();

	return qHash( ba );
}

