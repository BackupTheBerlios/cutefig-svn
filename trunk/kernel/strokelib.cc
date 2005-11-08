 
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

#include "strokelib.h"

ColorLib& ResLib<Stroke>::colorLib = ColorLib::instance();
GradLib& ResLib<Stroke>::gradLib = GradLib::instance();


ResLib<Stroke>& ResLib<Stroke>::instance()
{
        static ResLib<Stroke> inst;
        return inst;
}

bool ResLib<Stroke>::insert( const ResourceKey& key, const Stroke& stroke )
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


        return success;
}

bool ResLib<Stroke>::remove( const ResourceKey& key )
{
        if ( key.isBuiltIn() ) 
                return false;

        return colorLib.remove( key ) || gradLib.remove( key ); 

}

bool ResLib<Stroke>::contains( const ResourceKey& key ) const
{
        return colorLib.contains( key ) || gradLib.contains( key );
}

const Stroke ResLib<Stroke>::operator[]( const ResourceKey& key ) const
{
        if ( colorLib.contains( key ) )
                return Stroke( key, colorLib[key] );
        if ( gradLib.contains( key ) )
                return Stroke( key, gradLib[key] );

        return Stroke();
}

void ResLib<Stroke>::insertBuiltIn( const ResourceKey& key, const Stroke& data )
{
        switch ( data.type() ) {
            case Stroke::sColor:
                    colorLib.insertBuiltIn( key, data.color() ); break;
            case Stroke::sGradient:
                    gradLib.insertBuiltIn( key, data.gradient() ); break;
            default: break;
        }
}

int ResLib<Stroke>::count() const
{
        return colorLib.count() + gradLib.count();
}

ResourceKey ResLib<Stroke>::at( int i ) const 
{
        int c = colorLib.count();
        if ( i < c )
                return colorLib.at( i );

        i -= c;
        //c = gradLib.count();
        
        return gradLib.at( i );
}

int ResLib<Stroke>::indexOf( const ResourceKey& key ) const 
{
        if ( colorLib.contains( key ) )
                return colorLib.indexOf( key );

        return colorLib.count() + gradLib.indexOf( key );
}
