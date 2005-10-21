 
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

#ifndef resourcekey_h
#define resourcekey_h

#include <QString>
#include <QHash>

class ResourceKey;

uint qHash( const ResourceKey& key );
bool operator== ( const ResourceKey& k1, const ResourceKey& k2 );
bool operator<  ( const ResourceKey& k1, const ResourceKey& k2 );

class ResourceKey
{
public:
        enum Flags { Invalid = 0x00, BuiltIn = 0x01, InLib = 0x02, InFig = 0x04 };

        friend uint qHash( const ResourceKey& key );
        friend bool operator== ( const ResourceKey& k1, const ResourceKey& k2 );
        friend bool operator<  ( const ResourceKey& k1, const ResourceKey& k2 );

        ResourceKey() : keyString_(), flags_( Invalid ) {}
        ResourceKey( const QString& ks, Flags flags ) : keyString_( ks ), flags_( flags ) {}
        ResourceKey( const ResourceKey& o ) : keyString_( o.keyString_ ), flags_( o.flags_ ) {}
        const QString& keyString() const { return keyString_; }
        
        bool isBuiltIn() const { return flags_ & BuiltIn; }
        bool isToBeSaved() const { return flags_ > BuiltIn; }
        bool isValid() const { return flags_ != Invalid; }
        
private:
        QString keyString_;
        Flags flags_;
};

#define BUILTIN_KEY( key ) ResourceKey( key, ResourceKey::BuiltIn )

inline bool operator== ( const ResourceKey& k1, const ResourceKey& k2 )
{
        return k1.keyString_ == k2.keyString_ && k1.flags_ == k2.flags_;
}

inline bool operator< ( const ResourceKey& k1, const ResourceKey& k2 )
{
        if ( k1.flags_ < k2.flags_ )
                return true;

        if ( k1.flags_ > k2.flags_ )
                return false;

        return k1.keyString_ < k2.keyString_;
}

inline uint qHash( const ResourceKey& key )
{
        return qHash( qHash( key.keyString_ ) ^ key.flags_ );
}
        
        
#endif
