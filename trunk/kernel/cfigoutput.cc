 
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
#include "figure.h"
#include "allobjects.h"
#include "keywords.h"
#include "resourceio.h"
#include "streamops.h"

#include <QPolygonF>
#include <QDate>

#include <iomanip>

#include <QDebug>


CfigOutput::CfigOutput( QTextStream& ts, const Figure& f )
        : OutputBackend( ts, f )
{
}

void CfigOutput::outputRectangle( const Rectangle* r )
{
        drawObject_ = r;
        outputGenericData();
        fileStream_ << ' ' << r->angle() << ' '
                    << r->xCornerRad() << ' '
		    << (r->hasEqualCornerRadii() ? -1.0 : r->yCornerRad()) << "\n";
        outputPoints();
}

void CfigOutput::outputEllipse( const Ellipse* el )
{
        drawObject_ = el;
        outputGenericData();
        fileStream_.setRealNumberPrecision( 3 );
        fileStream_ << ' ' << el->isCircle() << ' ' 
                    << el->isSpecifiedByRadii() << ' ' << el->angle()
                    << "\n";
        outputPoints();
}

void CfigOutput::outputPolyline( const Polyline* pl )
{
        drawObject_ = pl;
        outputGenericData();
        outputArrows();
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

void CfigOutput::outputArc( const Arc* arc )
{
        drawObject_ = arc;
        outputGenericData();
        fileStream_ << ' ' << arc->rectSize()/2/unit() << ' ' << arc->angle()
                    << ' ' << (int)arc->arcFlag()  << ' ' << (int)arc->sweepFlag() << "\n";
        outputPoints();
}

void CfigOutput::outputBezierSpline( const BezierSpline* bs )
{
        drawObject_ = bs;
        outputGenericData();
        outputArrows();
        fileStream_ << "\n";
        outputPoints();
}

void CfigOutput::outputTextObject( const TextObject* to )
{
        drawObject_ = to;
	QString text = to->text();
	if ( text.isEmpty() )
		return;
	
        outputGenericData();

        const QFont& f = to->font();
        
        fileStream_ << ' ' << f.family() << ' ' << f.pointSize() << ' '
                    << to->alignment() << ' ' << percentEncode(text) << "\n";

        outputPoints();
}

void CfigOutput::outputCompound( const Compound* cd )
{
        const ObjectList& objects = cd->childObjects();
        fileStream_ << KWds::compound_begin() << "\n";
        foreach ( DrawObject* o, objects ) 
                o->outputToBackend( this );
        fileStream_ << KWds::compound_end() << "\n";
}

void CfigOutput::processOutput()
{
        outputHeader();
        outputResources();
        outputMetaData();
        figure_.outputObjects( this );
}


void CfigOutput::outputGenericData()
{
        const Pen& p = drawObject_->pen();

        if ( !drawObject_->comment().isEmpty() ) {
                QStringList comments = drawObject_->comment().split("\n");
		if ( comments.last().isEmpty() )
			comments.removeLast();
                foreach ( QString s, comments )
			fileStream_ << "# " << s << "\n";
        }
        
        fileStream_ << KWds::object() << ' ' << drawObject_->objectKeyword() << ' '
                    << drawObject_->points().size() << ' ';
        
        fileStream_ << p.width() << ' ' << p.dashesKey() << ' '
                    << (int)p.capStyle() << ' ' << (int)p.joinStyle() << ' '
                    << drawObject_->stroke() << ' ' << drawObject_->fill() << ' '
                    << drawObject_->depth();
}

void CfigOutput::outputPoints()
{
        foreach ( QPointF p, drawObject_->points() )
                fileStream_ << KWds::point() << ' ' << p.x()/unit() << ' ' << p.y()/unit() << "\n";
}

void CfigOutput::outputHeader()
{
        fileStream_ << KWds::CuteFig() << ' ' << KWds::version() << ' ' << Fig::version << "\n";
}

void CfigOutput::outputResources()
{
        ResourceSet rs = figure_.usedResources();

        for ( ResourceSet::const_iterator it = rs.begin() ; it != rs.end() ; ++it ) {
                ResourceIO* rIO = ResourceIOFactory::getResourceIO( it.key() );
                foreach ( const ResourceKey& key, it.value() )
                        rIO->outputResource( key, fileStream_ );
        }

        fileStream_ << KWds::no_more_resources() << "\n";
}


void CfigOutput::outputMetaData()
{
        fileStream_ << KWds::unit()          << ' ' << figure_.unitKey() << "\n"
                    << KWds::scale()         << ' ' << figure_.scale() << "\n"
                    << KWds::paper()         << ' ' << figure_.paperKey() << "\n"
                    << KWds::paper_orientation() << ' ' 
                    << (figure_.paperOrientation() == Fig::Landscape
                        ? KWds::landscape() : KWds::portrait()) << "\n";
        
        const QString& ath = figure_.author();
        if ( figure_.authorIsToBeSaved() && !ath.isEmpty() )
                fileStream_ << KWds::author() << ' ' << percentEncode(figure_.author()) << "\n";
        
        const QString& dsc = figure_.description();
        if ( !dsc.isEmpty() )
                fileStream_ << KWds::description() << ' ' << percentEncode(dsc) << "\n";
        
        fileStream_ << KWds::created()       << ' ' << figure_.creationDate() << "\n"
                    << KWds::last_modified() << ' ' << figure_.modificationDate() << "\n"
                    << KWds::metaData_end()  << "\n";
}


void CfigOutput::outputArrows()
{
        const ResourceKey& start = drawObject_->startArrowKey();
        const ResourceKey& end = drawObject_->endArrowKey();
        
        fileStream_ << ' ' << start;
        if ( start.isValid() ) {
                const Arrow& a = drawObject_->startArrow();
                fileStream_ << ' ' << a.width() << ' ' << a.length();
        }

        fileStream_ << ' ' << end;
        if ( end.isValid() ) {
                const Arrow& a = drawObject_->endArrow();
                fileStream_ << ' ' << a.width() << ' ' << a.length();
        }
        
}

inline double CfigOutput::unit()
{
	return figure_.unit();
}
