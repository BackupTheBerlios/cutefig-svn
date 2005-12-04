 
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

#ifndef rectangle_h
#define rectangle_h

#include "rectangloid.h"

CUTE_DECLARE_DRAWOBJECT( Rectangle, "rectangle", "&Rectangle" );

class Rectangle : public Rectangloid
{
        Q_OBJECT
        DRAW_OBJECT( Rectangle );
public:
        Rectangle( Figure* parent = 0 );
        Rectangle( const Rectangle* r );

        virtual bool pointHitsOutline( const QPointF& p, qreal tolerance ) const;

        virtual void outputToBackend( OutputBackend* ob );

private:
        virtual void setupWidthAndHeight();
        virtual void addPath();

};

// #include "dobjectfactory.h"

// class RectangleFactory : public DrawObjectFactory
// {
// public:
//         RectangleFactory() : DrawObjectFactory("rectangle") {}
//         virtual DrawObject* parseObject( std::istream& is, Figure* fig );
// };


#endif
