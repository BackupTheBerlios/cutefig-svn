 
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

#ifndef cfigoutput_h
#define cfigoutput_h

#include "outputbackend.h"
#include "reslib.h"

#include <ostream>
#include <QObject>
#include <QHash>

class CfigOutput : public OutputBackend
{
public:
        CfigOutput( std::ostream& ts, const Figure& f ) : OutputBackend( ts, f ) {}
        ~CfigOutput() {}
        
        virtual void outputRectangle( const Rectangle* rc );
        virtual void outputEllipse( const Ellipse* el );
        virtual void outputPolyline( const Polyline* pl );
        virtual void outputPolygon( const Polygon* pg );
        virtual void outputArc( const Arc* arc );
        virtual void outputBezierSpline( const BezierSpline* bs );
        virtual void outputTextObject( const TextObject* to );

        virtual void outputCompound( const Compound* cd );
        
        virtual void processOutput();

private:
        void outputGenericData();
        void outputArrows();
        void outputPoints();
        void outputHeader();
        void outputResources();

        static const char* objectString;

        const DrawObject* drawObject_;

        QHash<int,int> dashMap_;
};



#endif
