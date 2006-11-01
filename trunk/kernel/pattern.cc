
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


#include "pattern.h"
#include "geometry.h"
#include "drawobject.h"
#include "cfigoutput.h"


#include <QBrush>
#include <QPainter>
#include <QHash>
#include <QTextStream>

#include <cmath>

#include <QDebug>

Pattern::Pattern()
	: figure_()
{
}


Pattern::Pattern( const ObjectList& obs )
	: figure_()
{
        figure_.takeDrawObjects( obs );
}


void Pattern::setObjectList( const ObjectList& obs )
{
        figure_.clear();
        figure_.takeDrawObjects( obs );
}        

const QBrush Pattern::brush( const QRectF& r, const QMatrix& m ) const
{
        if ( figure_.isEmpty() )
                return QBrush();
        
        QSizeF bsize( m.mapRect( r ).size() ); 

        QPixmap brush( bsize.toSize() );
        brush.fill( Qt::transparent );

	QPainter p;
	p.begin( &brush );
        p.setRenderHints( QPainter::Antialiasing, true );
        p.setWorldMatrix( QMatrix( m.m11(), m.m12(), m.m21(), m.m22(), .0, .0 ) );
        
        double scale = hypot( m.m11(), m.m12() );
        QRectF pr = figure_.boundingRect();
        QSizeF ps = QSizeF( pr.right(), pr.bottom() ) * scale;
        
        for ( double x=0; x < bsize.width(); x += ps.width() )
                for ( double y=0; y < bsize.height(); y += ps.height() ) {
                        p.translate( x,y );
                        figure_.drawElements( &p );
                        p.translate( -x,-y );
                }
        
        p.end();
        
        return QBrush( brush );
}


#include "resourceio.h"
#include "parser.h"
#include "keywords.h"
#include "cfigoutput.h"
#include "streamops.h"

template<>
void ResLib<Pattern>::init() 
{
	insertBuiltIn( "nopattern", Pattern() );
}

template<>
ResourceKey ResLib<Pattern>::defaultKey()
{
	return ResourceKey::builtIn("nopattern");
}

template<>
const QString ResLib<Pattern>::resourceName()
{
        return "pattern";
}

template<>
bool TResourceIO<Pattern>::parseResource( const QString& itemType_, QTextStream& is )
{
        if ( itemType_.isEmpty() ) {
                ResourceKey key;
                is >> key;
                resource_.figure_.setUnit( key );
        } else {
                streamToBuffer( is );
                if ( !buffer().isEmpty() )
                        buffer() += "\n";
        }

        return true;
}

template<>
void TResourceIO<Pattern>::postProcessResource() 
{
        QTextStream ts( &buffer(), QIODevice::ReadOnly );
        errorString_.clear();
        resource_.figure_.clear();
        errorString_ = Parser::parseObjects( ts, &resource_.figure_ );
        clearBuffer();

        failed_ = !errorString_.isEmpty();
}

template<>
void TResourceIO<Pattern>::outputResourceBody( const Pattern& pat, QTextStream& ts ) const
{
        ts << pat.figure_.unitKey() << "\n";
        CfigOutput op( ts, pat.figure_ );
        pat.figure_.outputObjects( &op );
}


TResourceIOFactory<Pattern> rIOFinstance;

unsigned int qHash( const Pattern& p )
{
	if ( p.figure_.isEmpty() )
		return 0;

	QString str;
	QTextStream ts( &str, QIODevice::WriteOnly );
	CfigOutput op( ts, p.figure_ );
        p.figure_.outputObjects( &op );

	return qHash( str );
}
