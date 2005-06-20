 
/*****************************************************************************
**
**  @version $Id: cfigsave.cc 2 2005-06-15 20:01:37Z joh $
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


#include "cfigsave.h"
#include "allobjects.h"
#include "pen.h"

#include <QPolygonF>
#include <QTextStream>

const char* CfigSave::objectString = "object ";

void CfigSave::saveEllipse( Ellipse* el )
{
        drawObject_ = el;
        saveGenericData( "ellipse" );
        fileStream_.setRealNumberPrecision( 3 );
        fileStream_ << ' ' << el->isCircle() << ' ' 
                    << el->isSpecifiedByRadii() << ' ' << el->angle()
                    << "\n";
        savePoints();
}

void CfigSave::processSave()
{
        saveDashes();
        figure_.passObjects( this );
}


void CfigSave::savePolyline( Polyline* pl )
{
        drawObject_ = pl;
        saveGenericData( "polyline" );
        fileStream_ << "\n";
        savePoints();
}

void CfigSave::savePolygon( Polygon* pg )
{
        drawObject_ = pg;
        saveGenericData( "polygon" );
        fileStream_ << "\n";
        savePoints();
}

void CfigSave::saveGenericData( QString name )
{
        const Pen& p = drawObject_->pen();
        int dk = p.dashesKey();
        if ( dk >= 0 )
                dk = dashMap_[dk];
        
        QStringList comments = drawObject_->comment().split('\n');
        foreach ( QString s, comments ) 
                fileStream_ << "# " << s << '\n';

        fileStream_ << "object " << name << ' ' << drawObject_->points().size() << ' ';
        
        fileStream_ << p.width() << ' ' << dk << ' '
                     << (int)p.capStyle() << ' ' << (int)p.joinStyle() << ' '
                     << p.color().name() << ' ' << p.color().alphaF() << ' ';

        fileStream_ << drawObject_->fillColor().name() << ' ' 
                     << drawObject_->fillColor().alphaF() << ' '
                     << -1 << ' ' << drawObject_->depth();
}

void CfigSave::savePoints()
{
        foreach ( QPointF p, drawObject_->points() )
                fileStream_ << "point " << p.x() << ' ' << p.y() << "\n";
}

void CfigSave::saveDashes()
{
        DashesLib& dl = DashesLib::instance();
        dashMap_ = QHash<int,int>();

        int i = 0;
        foreach ( int key, figure_.dashList() ) {
                dashMap_[key] = i++;
                fileStream_ << "dashes";
                foreach( double d, dl[key] )
                        fileStream_ << " " << d;
                fileStream_ << "\n";
        }
}

