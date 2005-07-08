 
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


#include "reslibinit.h"

#include "reslib.h"
#include "typedefs.h"
#include "parser.h"

void ResLibInit::init()
{
        static bool called = false;
        if ( called )
                return;
        
        initDashes();
        called = true;
}

void ResLibInit::initDashes()
{
        DashesLib& dl = DashesLib::instance();
        
        dl << Parser::parseDashLine("1.0 3.0")
           << Parser::parseDashLine("3.0 3.0")
           << Parser::parseDashLine("3.0 3.0 1.0 3.0")
           << Parser::parseDashLine("3.0 3.0 1.0 3.0 1.0 3.0")
           << Parser::parseDashLine("3.0 3.0 1.0 3.0 1.0 3.0 1.0 3.0")
           << Parser::parseDashLine("3.0 3.0 3.0 3.0 1.0 3.0")
           << Parser::parseDashLine("3.0 3.0 3.0 3.0 3.0 3.0 1.0 3.0")
           << Parser::parseDashLine("3.0 3.0 3.0 3.0 1.0 3.0 1.0 3.0")
           << Parser::parseDashLine("0.2 3.8");
}
