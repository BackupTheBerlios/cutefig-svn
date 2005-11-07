 
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

#ifndef stdresio_h
#define stdresio_h

#include "resourceio.h"
#include "reslib.h"
#include "typedefs.h"


#include <istream>


#include <QColor>
class ColorIOFactory : public ResourceIOFactory 
{
public:
        ColorIOFactory() : ResourceIOFactory("color") {}
        virtual ResourceIO* newResourceIO() { return new TResourceIO<QColor>(); }
};



#include "gradient.h"
class GradientIOFactory : public ResourceIOFactory 
{
public:
        GradientIOFactory() : ResourceIOFactory("gradient") {}
        virtual ResourceIO* newResourceIO() { return new TResourceIO<Gradient>(); }
};



#include "pen.h"
#include <QVector>
class DashesIOFactory : public ResourceIOFactory
{
public:
        DashesIOFactory() : ResourceIOFactory("dashes") {}
        virtual ResourceIO* newResourceIO() { return new TResourceIO<Dashes>(); }
};


#endif
