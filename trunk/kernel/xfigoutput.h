 
/*****************************************************************************
**
**  @version $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2005 Johannes Mueller, joh@users.berlios.net
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

#ifndef xfigoutput_h
#define xfigoutput_h

#include "outputbackend.h"

#include <QTextStream>
#include <QObject>
#include <list>

class XFigOutput : public OutputBackend
{
public:
        XFigOutput( QTextStream& ts, Figure& f );
        ~XFigOutput() { };

        virtual void outputEllipse( Ellipse* el );
        virtual void outputPolyline( Polyline* pl );
        virtual void outputPolygon( Polygon* pg );
        
        virtual void outputHeader();
        virtual void outputTrailer();

private:
        void outputGenericData();
        
        QTextStream* fileStream_;
        DrawObject* drawObject_;

        
};

#endif
