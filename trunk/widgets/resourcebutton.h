 
/*****************************************************************************
**
**  @version $Id: colorbutton.h 16 2005-06-26 22:00:14Z joh $
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

#ifndef resourcebutton_h
#define resourcebutton_h


#include "resourceiconengine.h"
#include "reslibeditor.h"

#include <QPushButton>
#include <QIcon>

class AbstractResourceButton : public QPushButton
{
        Q_OBJECT
public:
        AbstractResourceButton( const ResourceKey& key, QWidget* parent = 0 )
                : QPushButton( parent ),
                  key_( key )
        {}

        ResourceKey key() const { return key_; }
        
signals:
        void resourceChanged();
        
public slots:
        virtual void changeResource() = 0;

protected:
        ResourceKey key_;
};


template<class Resource> class ResourceButton : public AbstractResourceButton
{
public:
        ResourceButton( const ResourceKey& key, QWidget* parent = 0 );
        ResourceButton( const Resource& resource, QWidget* parent = 0 );
        
        ~ResourceButton() {}

        void setResource( const Resource& resource );
        Resource getResource() const { return resource_; }

        virtual void changeResource();
        
private:
        const ResLib<Resource>& resourceLib_;
        Resource resource_;
};

template<class Resource>
ResourceButton<Resource>::ResourceButton( const Resource& resource, QWidget* parent )
        : AbstractResourceButton( ResourceKey(), parent ),
          resourceLib_( ResLib<Resource>::instance() ),
          resource_( resource )
{
        setIcon( QIcon( new ResourceIconEngine<Resource>( resource_ ) ) );
        connect( this, SIGNAL( clicked() ), this, SLOT( changeResource() ) );
}

template<class Resource>
ResourceButton<Resource>::ResourceButton( const ResourceKey& key, QWidget* parent )
        : AbstractResourceButton( ResourceKey(), parent ),
          resourceLib_( ResLib<Resource>::instance() ),
          resource_( resourceLib_[key] )
{
        setIcon( QIcon( new ResourceIconEngine<Resource>( resource_ ) ) );
        connect( this, SIGNAL( clicked() ), this, SLOT( changeResource() ) );
}

template<class Resource>
void ResourceButton<Resource>::setResource( const Resource& resource )
{
        resource_ = resource;
        setIcon( QIcon( new ResourceIconEngine<Resource>( resource_ ) ) );
}


template<class Resource>
void ResourceButton<Resource>::changeResource()
{
        bool ok;

        ResourceKey newkey = ReslibEditor<Resource>::getResource( key_, &ok, this );
        
        if ( ok ) {
                key_ = newkey;
                setResource( resourceLib_[newkey] );
                emit resourceChanged();
        }
}


#include "colordialog.h"

template<> inline void ResourceButton<QColor>::changeResource()
{
        bool ok;

        QColor newcolor = ColorDialog::getColor( resource_, &ok, this );
        
        if ( ok ) {
                setResource( newcolor );
                emit resourceChanged();
        }
}
        
#endif
