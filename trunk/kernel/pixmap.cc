 
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

#include <QByteArray>
#include <QDataStream>
#include <QHash>


Pixmap::Pixmap()
	: fileName_(),
	  pixmap_( 0 )
{
}

Pixmap::~Pixmap()
{
        delete pixmap_;
}

void Pixmap::setPixmap( const QPixmap& pm )
{
	fileName_.clear();
	pixmap() = pm;
}

bool Pixmap::loadFromFile( const QString& filename )
{
	if ( filename == fileName_ )
		return true;
	
	QPixmap pm;
	if ( !pm.load( filename ) )
		return false;

	fileName_ = filename;
	pixmap() = pm;

	return true;
}

bool Pixmap::isFromFile() const
{
	return !fileName_.isEmpty();
}

const QPixmap Pixmap::qpixmap() const
{
        if ( !pixmap_ )
                return QPixmap();
        else
                return *pixmap_;
}

QPixmap& Pixmap::pixmap()
{
        if ( !pixmap_ )
                pixmap_ = new QPixmap;
        
        return *pixmap_;
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
bool TResourceIO<Pixmap>::parseResource( const QString&, QTextStream& is )
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
void TResourceIO<Pixmap>::outputResourceBody( const Pixmap& res, QTextStream& ts ) const
{
	if ( res.isFromFile() ) {
		ts << res.fileName() << "\n";
		return;
	}
}

static TResourceIOFactory<Pixmap> rIOFinstance;


int qHash( const Pixmap& pm )
{
	if ( pm.isFromFile() || !pm.pixmap_ )
		return 0;

	QByteArray ba;
	QDataStream st( &ba, QIODevice::WriteOnly );

	st << pm.qpixmap();

	return qHash( ba );
}

Pixmap::Pixmap( const Pixmap& other )
        : fileName_( other.fileName_ ),
          pixmap_( other.pixmap_ ? new QPixmap( *other.pixmap_ ) : 0 )
{
}

Pixmap& Pixmap::operator= ( const Pixmap& other )
{
        if ( &other == this )
                return *this;
        
        if ( pixmap_ )
                delete pixmap_;
        
        pixmap_ = other.pixmap_ ? new QPixmap( *other.pixmap_ ) : 0;
        fileName_ = other.fileName_;

        return *this;
}
