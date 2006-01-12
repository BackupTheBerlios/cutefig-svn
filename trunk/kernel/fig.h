 
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

#ifndef fig_h
#define fig_h

#include <QString>

/** \namespace Fig
 *  \brief contains some global constants and enums
 */

namespace Fig
{
        const double inch2pix = 72.0;
        const double pix2inch = 1/inch2pix;
        const double inch2cm = 2.54;
        const double cm2inch = 1/inch2cm;
        const double pix2cm = inch2cm*pix2inch;
        const double cm2pix = 1/pix2cm;

        enum PointFlag { 
                None      = 0x0000,
                Normal    = 0x0001,
                Cancel    = 0x0002,
                Final     = 0x0004,
                Straight  = 0x0008,
                Special1  = 0x0010, 
                Special2  = 0x0020
        }; //!< Flags that can be passed to a DrawObject being edited.

        const double version = 0.1;
};

namespace Msgs
{
        const QString projectName( QT_TR_NOOP("The CuteFig project") );
        const QString version( QT_TR_NOOP("version 0.01") );
        const QString majorAuthor("Johannes Mueller");
        const QString bugAddress("joh@users.berlios.de");
        const QString bugreportsTo( QString( QT_TR_NOOP("Please report bugs to %1") )
                                    .arg( bugAddress ) );
}

#endif
