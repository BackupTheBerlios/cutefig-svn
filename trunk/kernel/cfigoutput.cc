 
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


#include "cfigoutput.h"
#include "allobjects.h"
#include "resourceio.h"
#include "streamops.h"

#include <QPolygonF>
#include <ostream>
#include <iomanip>

#include <QDebug>

const char* CfigOutput::objectString = "object ";

void CfigOutput::outputRectangle( const Rectangle* r )
{
        drawObject_ = r;
        outputGenericData();
        fileStream_ << "\n";
        outputPoints();
}

void CfigOutput::outputEllipse( const Ellipse* el )
{
        drawObject_ = el;
        outputGenericData();
        fileStream_ << std::setprecision( 3 );
        fileStream_ << ' ' << el->isCircle() << ' ' 
                    << el->isSpecifiedByRadii() << ' ' << el->angle()
                    << "\n";
        outputPoints();
}

void CfigOutput::outputPolyline( const Polyline* pl )
{
        drawObject_ = pl;
        outputGenericData();
        fileStream_ << "\n";
        outputPoints();
}

void CfigOutput::outputPolygon( const Polygon* pg )
{
        drawObject_ = pg;
        outputGenericData();
        fileStream_ << "\n";
        outputPoints();
}

void CfigOutput::outputTextObject( const TextObject* to )
{
        drawObject_ = to;
        outputGenericData();

        const QFont& f = to->font();
        
        fileStream_ << f.family() << ' ' << f.pointSize() << ' '
                    << f.italic() << ' ' << f.weight() << ' '
                    << to->alignment() << ' ' << to->text() << "\n";

        outputPoints();
}

void CfigOutput::outputCompound( const Compound* cd )
{
        const ObjectList& objects = cd->childObjects();
        fileStream_ << "compound_begin\n";
        foreach ( DrawObject* o, objects ) 
                o->outputToBackend( this );
        fileStream_ << "compound_end\n";
}

void CfigOutput::processOutput()
{
        outputResources();
        figure_.outputObjects( this );
}


std::ostream& operator<< ( std::ostream& ts, const ResourceKey& key )
{
        if ( !key.isValid() ) {
                ts << '%';
                return ts;
        }
        
        if ( key.isBuiltIn() )
                ts << '&';
        else
                ts << '*';

        ts << key.keyString();

        return ts;
}

std::ostream& operator<< ( std::ostream& ts, const Stroke& st )
{
        if ( st.isHardColor() )
                ts << st.color();
        else
                ts << st.key() << ' ' << st.typeString();

        return ts;
}

void CfigOutput::outputGenericData()
{
        const Pen& p = drawObject_->pen();

        if ( !drawObject_->comment().isEmpty() ) {
                QStringList comments = drawObject_->comment().split('\n');
                foreach ( QString s, comments ) 
                        fileStream_ << "# " << s << '\n';
        }
        
        fileStream_ << "object " << drawObject_->objectKeyWord() << ' '
                    << drawObject_->points().size() << ' ';
        
        fileStream_ << p.width() << ' ' << p.dashesKey() << ' '
                    << (int)p.capStyle() << ' ' << (int)p.joinStyle() << ' '
                    << drawObject_->stroke() << ' ' << drawObject_->fill() << ' '
                    << drawObject_->depth();
}

void CfigOutput::outputPoints()
{
        foreach ( QPointF p, drawObject_->points() )
                fileStream_ << "point " << p.x() << ' ' << p.y() << "\n";
}

void CfigOutput::outputResources()
{
        ResourceSet rs = figure_.usedResources();

        for ( ResourceSet::const_iterator it = rs.begin() ; it != rs.end() ; ++it ) {
                ResourceIO* rIO = ResourceIOFactory::getResourceIO( it.key() );
                foreach ( const ResourceKey& key, it.value() )
                        rIO->outputResource( key, fileStream_ );
        }
}
