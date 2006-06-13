 
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

#include "initialiser.h"
#include "allobjects.h"
#include "resourceio.h"

#include <QDebug>

template<>
void Initialiser::AutoHash<ObjectHandler>::init()
{
        static TObjectHandler<Rectangle> rcf;
        static TObjectHandler<Ellipse> elf;
        static TObjectHandler<Polyline> plf;
        static TObjectHandler<Polygon> pgf;
        static TObjectHandler<Arc> agf;
        static TObjectHandler<BezierSpline> bsf;
        static TObjectHandler<TextObject> tgf;
}

template<>
void Initialiser::AutoHash<ResourceIOFactory>::init()
{
        static TResourceIOFactory<QColor> ciof;
        static TResourceIOFactory<Gradient> giof;
        static TResourceIOFactory<Dashes> diof;
}

