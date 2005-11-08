 
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

#ifndef strokelib_h
#define strokelib_h

#include "reslib.h"
#include "stroke.h"
#include "gradient.h"
#include "typedefs.h"

#include <QColor>


inline int qHash( const QColor& ) 
{
        return 0;
}


template<> class ResLib<Stroke> 
{
public:
        friend class ResLibInit;

        static ResLib<Stroke>& instance();
        
        
        bool insert( const ResourceKey& key, const Stroke& stroke );
        bool remove( const ResourceKey& key );
        
        const Stroke operator[]( const ResourceKey& key ) const;
        
        bool contains( const ResourceKey& key ) const;
        
//        const ResourceKeyList& keys() const { return keys_; }

//        QList<Resource> resources() const { return map_.values(); }
        int count() const; 
        ResourceKey at( int i ) const;
        int indexOf( const ResourceKey& key ) const;

private:
        ResLib<Stroke>() {}
        ResLib<Stroke>( const ResLib<Stroke>& ) {}
        
        void insertBuiltIn( const ResourceKey& key, const Stroke& data );
        
        static ColorLib& colorLib;
        static GradLib& gradLib;

        template<typename Resource> friend class ResLib;
};

#endif