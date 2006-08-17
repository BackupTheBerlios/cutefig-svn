 
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

#include "streamops.h"

#include <QString>
#include <QHash>

class QSizeF;
template<typename ValueType> class ValueHash;

template<typename ValueType> void fillHash( QHash<QString,ValueType>& );

// typedef ValueHash<double> Unit;
// typedef ValueHash<QSizeF> Paper;

template<typename ValueType> class ValueHash 
{
public:
        ValueHash( const QString& name = defaultName() );
        ValueHash( const QString& name, const ValueType& value );

        explicit ValueHash( std::istream& is );
        
        ValueHash( const ValueHash<ValueType>& other );
        ~ValueHash() {}

        ValueType value() const { return value_; }
        operator ValueType() const { return value_; }
        QString name() const { return name_; }

        bool isValid() const { return valid_; }
        
        ValueHash<ValueType>& operator=( const ValueHash<ValueType>& other );

        static QList<QString> names() { return hash().keys(); }
        
private:
        QString name_;
        ValueType value_;

        static QHash<QString,ValueType>& hash();
        
        static const QString defaultName();
//        static const ValueType defaultValue();

        bool valid_;
};



template<typename ValueType>
ValueHash<ValueType>::ValueHash( const QString& name )
        : name_( defaultName() ),
          value_( hash()[name_] ),
          valid_( false )
{
        if ( hash().contains( name ) ) {
                value_ = hash()[name];
                name_ = name;
                valid_ = true;
        }
}

template<typename ValueType>
ValueHash<ValueType>::ValueHash( const QString& name, const ValueType& value )
        : name_( name ),
          value_( value ),
          valid_( true )
{
        if ( hash().contains( name ) && hash()[name] != value_ )
                valid_ = false;
}


template<typename ValueType>
ValueHash<ValueType>::ValueHash( std::istream& is )
        : name_(),
          value_(),
          valid_( true )
{
        is >> name_ >> value_;
        if ( is.fail() || ( hash().contains( name_ ) && hash()[name_] != value_ ) )
                valid_ = false;
}



template<typename ValueType>
ValueHash<ValueType>::ValueHash( const ValueHash<ValueType>& other )
        : name_( other.name_ ),
          value_( other.value_ ),
          valid_( other.valid_ )
{
}

template<typename ValueType>
QHash<QString,ValueType>& ValueHash<ValueType>::hash()
{
        static QHash<QString,ValueType> h;
        if ( h.isEmpty() )
                fillHash( h );

        return h;
}

template<typename ValueType>
ValueHash<ValueType>& ValueHash<ValueType>::operator=( const ValueHash<ValueType>& other )
{
        if ( this != &other ) {
                name_ = other.name_;
                value_ = other.value_;
                valid_ = other.valid_;
        }

        return *this;
}


template<typename ValueType>
std::ostream& operator<< ( std::ostream& os, const ValueHash<ValueType>& vh )
{
        os << vh.name() << " " << vh.value();
        return os;
}


template<typename ValueType>
std::istream& operator>> ( std::istream& is, ValueHash<ValueType>& vh ) 
{
        QString n;
        is >> n;
        vh = ValueHash<ValueType>( n );
        if ( !vh.isValid() )
                is.setstate( is.rdstate() | std::istream::failbit );

        return is;
}


#endif
