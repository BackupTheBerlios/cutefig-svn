 
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

#ifndef viewbase_h
#define viewbase_h

//#include <QWidget>

class DrawObject;
class Controler;
class Figure;

class QPainter;

class ViewBase
{
public:
        ViewBase( Controler* c, const Figure* f ) 
                : controler_( c ),
                  figure_( f )
        {}
        
        virtual ~ViewBase() {}

        virtual void updateFigure( bool tentative = false ) = 0;

        virtual void drawSelection( QPainter* p ) const = 0;
        virtual void drawObjectsMetaData( QPainter* p, const DrawObject* o ) const = 0;
        virtual void drawObjectsPoints( QPainter* p, const DrawObject* o ) const = 0;

        virtual void updateFigureMetaData() = 0;
        
protected:
        Controler* controler_;
        const Figure* figure_;
};

#endif
