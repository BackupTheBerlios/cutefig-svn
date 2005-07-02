 
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

#include "percentvalidatior.h"

void PercentValidator::setBottom( int bottom )
{
        if ( bottom >= 1 )
                QIntValidator::setBottom( bottom );
}

void PercentValidator::setRange( int min, int max )
{
        setTop( max );
        setBottom( min );
}

QValidator::State PercentValidator::validate( QString& input, int& pos ) const
{
        QString myInput = input.trimmed();
        if ( myInput.endsWith('%') )
                myInput.chop( 1 );
        
        return QIntValidator::validate( myInput, pos );
}

void PercentValidator::fixup( QString& input )
{
        int pos;
        if ( validate( input, pos ) == QValidator::Acceptable && !input.endsWith('%') )
                input.append('%');
}
