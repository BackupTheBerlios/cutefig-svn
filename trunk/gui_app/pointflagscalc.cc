 
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

#include "pointflagscalc.h"

Fig::PointFlags PointFlagsCalc::calcPointFlags( Qt::MouseButtons b, Qt::KeyboardModifiers m )
{
        Fig::PointFlags pf = Fig::Normal;
        if ( b & Qt::RightButton )
                pf |= Fig::Cancel;
        if ( b & Qt::MidButton )
                pf |= Fig::Final;

        if ( m & Qt::ShiftModifier )
                pf |= Fig::Straight;
        if ( m & Qt::ControlModifier )
                pf |= Fig::Special1;
        if ( m & Qt::AltModifier )
                pf |= Fig::Special2;

        return pf;
}

Qt::KeyboardModifiers PointFlagsCalc::toKbdModifiers( Fig::PointFlags f )
{
        Qt::KeyboardModifiers m = Qt::NoModifier;

        if ( f & Fig::Straight )
                m |= Qt::ShiftModifier;
        if ( f & Fig::Special1 )
                m |= Qt::ControlModifier;
        if ( f & Fig::Special2 )
                m |= Qt::AltModifier;

        return m;
}

Qt::MouseButton PointFlagsCalc::toMouseButton( Fig::PointFlags f )
{
        if ( f & Fig::Final )
                return Qt::MidButton;
        if ( f & Fig::Cancel )
                return Qt::RightButton;

        return Qt::LeftButton;
}
