 
/*****************************************************************************
**
**  $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2004 Johannes Mueller, johmue@users.sourceforge.net
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

#ifndef resource_h
#define resource_h

template<class T> class Resource
{
        enum Flags { Invalid = 0x00, InFigure = 0x01, BuiltIn = 0x02, InLib = 0x04 };

public:
        Resource<T>() : flags_( Invalid ) {}
        static Resource<T> makeFigureResource( T data ) { return Resource<T>( data, InFigure ); }
        static Resource<T> makeLibResource( T data ) { return Resource<T>( data, InLib ); }

        friend class ResLibInit;
        
        const T& getData() const { return data_; }
        T* data();
        
        bool isBuiltIn() const { return flags_ & BuiltIn; }
        bool isInLib() const { return flags_ & InLib; }
        bool isInFigure() const { return flags_ & InFigure; }

        operator bool () const { return !flags_; }
        
private:
        Resource<T>( T data, Flags flags ) : flags_( flags ), data_( data ) {}
        static Resource<T> makeBuiltInResource( T data ) { return Resource<T>( data, BuiltIn ); }

        Flags flags_;

        T data_;
};


#endif
