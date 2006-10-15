 
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
	  pixmap_()
{
}

void Pixmap::setPixmap( const QPixmap& pm )
{
	fileName_.clear();
	pixmap_ = pm;
}

bool Pixmap::loadFromFile( const QString& filename )
{
	if ( filename == fileName_ )
		return true;
	
	QPixmap pm;
	if ( !pm.load( filename ) )
		return false;

	fileName_ = filename;
	pixmap_ = pm;

	return true;
}

bool Pixmap::isFromFile() const
{
	return !fileName_.isEmpty();
}



template<> void ResLib<Pixmap>::init()
{
}

template<> ResourceKey ResLib<Pixmap>::defaultKey()
{
        return ResourceKey();
}


int qHash( const Pixmap& pm )
{
	if ( pm.isFromFile() )
		return 0;

	QByteArray ba;
	QDataStream st( &ba, QIODevice::WriteOnly );

	st << pm.qpixmap();

	return qHash( ba );
}

