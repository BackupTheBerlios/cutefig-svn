 
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

#ifndef length_h
#define length_h

#include "reslib.h"

class Length
{
public:
        Length( double value = 1.0 );
        Length( double value, const ResourceKey& key );
        Length( const Length& other );

        ~Length();
        
        operator double() const;
        double valueInBase() const { return value_; }

        Length& operator= ( const Length& other );
        
private:
        double value_;

        AbstractResourceUser* baseLengthRU_;
        static ResourceUser<Length>* baseLength();
};


inline
unsigned int qHash( const Length& ) { return 0; }

#endif
