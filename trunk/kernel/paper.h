 
/*****************************************************************************
**
**  @version $Id: cutefig.cc 114 2006-08-17 18:31:18Z joh $
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

#ifndef paper_h
#define paper_h

#include "length.h"

#include <QSizeF>


class Paper
{
public:
        Paper();
        Paper( QSizeF size, const ResourceKey& key );
        Paper( const Paper& other );

        QSizeF size() const;
        QSizeF sizeInUnit() const { return size_; }

private:
        QSizeF size_;
        ResourceUser<Length> unit_;
};

inline
unsigned int qHash( const Paper& ) { return 0; }


#endif
