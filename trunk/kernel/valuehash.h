 
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

#ifndef valuehash_h
#define valuehash_h

#include <QString>
#include <QHash>

class QSizeF;
template<typename ValueType> class ValueHash;

typedef ValueHash<double> Unit;
typedef ValueHash<QSizeF> Paper;

template<typename ValueType> class ValueHash 
{
public:
        ValueHash( const QString& name );
        ValueHash( const ValueHash<ValueType>& other );
        ~ValueHash() {}

        ValueType value() { return value_; }
        QString name() { return name_; }

private:
        ValueHash<ValueType>& operator=( const ValueHash<ValueType>& other );

        ValueType value_;
        QString name_;

        static QHash<QString,ValueType>& hash();
        
        static const QString defaultName();
        static const ValueType defaultValue();
};



template<typename ValueType>
ValueHash<ValueType>::ValueHash( const QString& name )
        : value_( defaultValue() ),
          name_( defaultName() )
{
        if ( hash().contains( name ) ) {
                value_ = hash()[name];
                name_ = name;
        }
}

template<typename ValueType>
ValueHash<ValueType>::ValueHash( const ValueHash<ValueType>& other )
        : value_( other.value_ ),
          name_( other.name_ )
{
}

template<typename ValueType> void fillHash( QHash<QString,ValueType>& );

template<typename ValueType>
QHash<QString,ValueType>& ValueHash<ValueType>::hash()
{
        static QHash<QString,ValueType> h;
        if ( h.isEmpty() )
                fillHash( h );

        return h;
}

#endif
