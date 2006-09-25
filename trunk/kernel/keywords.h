 
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

#ifndef keywords_h
#define keywords_h

struct KWds
{
        static char* CuteFig() { return "CuteFig"; }
        static char* version() { return "version"; }

        static char* metaData_end() { return "metaData_end"; }
        static char* scale() { return "scale"; }
        static char* paper() { return "paper"; }
        static char* unit() { return "unit"; }

        static char* paper_orientation() { return "paper_orientation"; }
        static char* portrait() { return "portrait"; }
        static char* landscape() { return "landscape"; }

        static char* author() { return "author"; }
        static char* description() { return "description"; }

        static char* created() { return "created"; }
        static char* last_modified() { return "last_modified"; }
        
        static char* resource() { return "resource"; }
        static char* resource_end() { return "resource_end"; }
        static char* no_more_resources() { return "no_more_resources"; }
        
        static char* object() { return "object"; }
        static char* compound_begin() { return "compound_begin"; }
        static char* compound_end() { return "compound_end"; }
        static char* point() { return "point"; }
};


#endif
