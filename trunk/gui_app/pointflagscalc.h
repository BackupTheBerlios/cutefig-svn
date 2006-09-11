 
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

#ifndef pointflagscalc_h
#define pointflagscalc_h

#include "fig.h"

#include <Qt>

class PointFlagsCalc 
{
public:
        //! calculates the Fig::PointFlags out of m and b.
        static Fig::PointFlags calcPointFlags( Qt::MouseButtons b, Qt::KeyboardModifiers m );
        static Qt::KeyboardModifiers toKbdModifiers( Fig::PointFlags f );
        static Qt::MouseButton toMouseButton( Fig::PointFlags f );
        
private:
        PointFlagsCalc();
        PointFlagsCalc( const PointFlagsCalc& );
};


#endif
