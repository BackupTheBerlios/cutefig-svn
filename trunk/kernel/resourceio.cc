 
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

#include "resourceio.h"
#include "parser.h"

#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QFile>
#include <QCoreApplication>




ResourceIOFactory::ResourceIOFactory( const QString& kw, AbstractResLib& rl )
        : resLib_( &rl )
{
        rIOFHash()[kw] = this;
}


ResourceIO* ResourceIOFactory::getResourceIO( const QString& keyWord )
{
        ResourceIOFactory* f = rIOFHash()[keyWord];

        if ( !f )
                return 0;
        
        ResourceIO* rio = f->newResourceIO();
        rio->keyWord_ = keyWord;
        return rio;
}


AbstractResLib* ResourceIOFactory::getResLibInstance( const QString& keyWord )
{
        ResourceIOFactory* f = rIOFHash()[keyWord];

        if ( !f )
                return 0;

        return f->resLibInstance();
}


static const QString resourceFileName()
{
        QSettings s;
        QString resfile( QFileInfo(s.fileName()).path() + QDir::separator() + "resourcelib.cfrl" );

        return s.value( "resfiles", QVariant( resfile ) ).toString();
}

void ResourceIOFactory::saveResLibs()
{
	QFile fl( resourceFileName() );
	if ( !fl.open( QFile::WriteOnly | QFile::Truncate ) ) {
		return;
	}
	
	QTextStream ts( &fl );
        
        foreach ( ResourceIOFactory* f, rIOFHash().values() ) {
                if ( f )
                        f->resLibInstance()->save( f->newResourceIO(), ts );
        }
        
}

QString ResourceIOFactory::readResLibs()
{
        QFile f( resourceFileName() );

        if ( !f.open( QIODevice::ReadOnly ) )
                return QString();
        
        QTextStream ts( &f );
        return Parser::parseResLibs( ts );
}

QHash<QString,ResourceIOFactory*>& ResourceIOFactory::rIOFHash()
{
        static QHash<QString,ResourceIOFactory*> h;
        return h;
}


QString ResourceIO::tr( const char* text )
{
	return QCoreApplication::translate( "ResourceIO", text );
}

void ResourceIO::streamToBuffer( QTextStream& ts )
{
        ts.seek( 0 );
        QString s = ts.readAll();
        if ( !s.isEmpty() )
                if ( !buffer_.isEmpty() )
                        buffer_ += "\n";
                buffer_ += s;
}

void ResourceIO::clearBuffer()
{
        buffer_.clear();
}

