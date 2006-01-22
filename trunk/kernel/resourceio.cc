 
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
#include "initialiser.h"
#include "parser.h"

#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>

#include <fstream>
//QHash<QString,ResourceIOFactory*> ResourceIOFactory::rIOFHash_;

Initialiser::AutoHash<ResourceIOFactory> ResourceIOFactory::rIOFHash_;

ResourceIOFactory::ResourceIOFactory( const QString& kw, AbstractResLib& rl )
        : resLib_( &rl )
{
        rIOFHash_[kw] = this;
}


ResourceIO* ResourceIOFactory::getResourceIO( const QString& keyWord )
{
        ResourceIOFactory* f = rIOFHash_[keyWord];

        if ( !f )
                return 0;
        
        ResourceIO* rio = f->newResourceIO();
        rio->keyWord_ = keyWord;
        return rio;
}


AbstractResLib* ResourceIOFactory::getResLibInstance( const QString& keyWord )
{
        ResourceIOFactory* f = rIOFHash_[keyWord];

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
        std::ofstream ts( resourceFileName().toLocal8Bit().constData() );
        
        foreach ( ResourceIOFactory* f, rIOFHash_.objects() )
                f->resLibInstance()->save( f->newResourceIO(), ts );
}

void ResourceIOFactory::readResLibs()
{
        QFile f( resourceFileName() );

        if ( f.open( QIODevice::ReadOnly ) ) {
                QTextStream ts( &f );
                Parser::parseResLibs( ts );
        }
}

