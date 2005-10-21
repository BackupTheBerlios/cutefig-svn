 
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

#ifndef strokelib_h
#define strokelib_h

#include "reslib.h"
#include "stroke.h"
#include "gradient.h"
#include "typedefs.h"

#include <QColor>

// template<> class ResLib<QColor> 
// {
//         friend class ResLib<Stroke>;
// };


template<> class ResLib<Stroke> 
{
public:
        friend class ResLibInit;

        static ResLib<Stroke>& instance()
        {
                static ResLib<Stroke> inst;
                return inst;
        };
        
        bool insert( const ResourceKey& key, const Stroke& stroke )
        {
                if ( key.isBuiltIn() )
                        return false;

                bool success = false;
                
                switch ( stroke.type() ) {
                    case Stroke::sColor:
                            success = colorLib.insert( key, stroke.color() ); break;
                    case Stroke::sGradient:
                            success = gradLib.insert( key, stroke.gradient() ); break;
                    default: break;
                }

                if ( success )
                        keys_ << key;

                return success;
        }

        bool remove( const ResourceKey& key )
        {
                if ( key.isBuiltIn() ) 
                        return false;

                bool success = colorLib.remove( key ) || gradLib.remove( key ); 

                if ( success )
                        keys_.removeAll( key );

                return success;
        }
        
        const Stroke operator[]( const ResourceKey& key ) const
        {
                if ( colorLib.contains( key ) )
                        return Stroke( key, colorLib[key] );
                if ( gradLib.contains( key ) )
                        return Stroke( key, gradLib[key] );

                return Stroke();
        }
        
//        const ResourceKey key( const Resource& data ) const { return map_[data]; }
        bool contains( const ResourceKey& key ) const
        {
                return keys_.contains( key );
        }               

        const ResourceKeyList& keys() const { return keys_; }

//        QList<Resource> resources() const { return map_.values(); }
        
private:
        ResLib<Stroke>() : keys_() {}
        ResLib<Stroke>( const ResLib<Stroke>& ) {}
        
        void insertBuiltIn( const ResourceKey& key, const Stroke& data )
        {
                switch ( data.type() ) {
                    case Stroke::sColor:
                            colorLib.insertBuiltIn( key, data.color() ); break;
                    case Stroke::sGradient:
                            gradLib.insertBuiltIn( key, data.gradient() ); break;
                    default: break;
                }

                keys_ << key;
       }
        
//        QMap<ResourceKey, Resource> map_;
        ResourceKeyList keys_;
        
        static ColorLib& colorLib;
        static GradLib& gradLib;
};


#endif
