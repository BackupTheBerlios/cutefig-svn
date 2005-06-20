 
/*****************************************************************************
**
**  @version $Id: cfigsave.h 2 2005-06-15 20:01:37Z joh $
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

#ifndef cfigsave_h
#define cfigsave_h

#include "savebackend.h"
#include "reslib.h"

#include <QTextStream>
#include <QObject>
#include <QHash>

class QPolygonF;

class CfigSave : public SaveBackend
{
public:
        CfigSave( QTextStream& ts, const Figure& f ) : SaveBackend( ts, f ) {}
        ~CfigSave() {}

        virtual void saveEllipse( Ellipse* el );
        virtual void savePolyline( Polyline* pl );
        virtual void savePolygon( Polygon* pg );
        
        virtual void processSave();

private:
        void saveGenericData( QString s );
        void savePoints();
        void saveDashes();

        static const char* objectString;

        DrawObject* drawObject_;

        QHash<int,int> dashMap_;

//        ResourceTable dashTable_;
        
};

#endif
