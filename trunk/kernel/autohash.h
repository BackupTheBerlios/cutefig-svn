 
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

#ifndef autohash_h
#define autohash_h

#include <QHash>
#include <QString>

template<typename Key, typename Value> class AutoHash
{
public:
	AutoHash();

	const Value operator[]( const Key& k ) const { return hash_[k]; }
	Value& operator[]( const Key& k ) { return hash_[k]; }

	const Key key( const Value& value ) const { return hash_.key( value ); }
	
private:
	QHash<Key,Value> hash_;

	void init();
};


template<typename Key, typename Value> AutoHash<Key, Value>::AutoHash()
	: hash_()
{
	init();
}

	

#endif
