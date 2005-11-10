 
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
        bool definedByKey_;
};


template<class Resource> class ResourceButton : public AbstractResourceButton
{
public:
        ResourceButton( const ResourceKey& key, QWidget* parent = 0 );
        ResourceButton( const Resource& resource, QWidget* parent = 0 );
        
        ~ResourceButton() {}

        void setResource( const Resource& resource );
        void setResource( const ResourceKey& key );
        Resource getResource() const { return resource_; }

        virtual void changeResource();
        
private:
        ResLib<Resource>& resourceLib_;
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
        definedByKey_ = false;
}

template<class Resource>
ResourceButton<Resource>::ResourceButton( const ResourceKey& key, QWidget* parent )
        : AbstractResourceButton( ResourceKey(), parent ),
          resourceLib_( ResLib<Resource>::instance() ),
          resource_( resourceLib_[key] )
{
        setIcon( QIcon( new ResourceIconEngine<Resource>( resource_ ) ) );
        connect( this, SIGNAL( clicked() ), this, SLOT( changeResource() ) );
        definedByKey_ = true;
}

template<class Resource>
void ResourceButton<Resource>::setResource( const Resource& resource )
{
        resource_ = resource;
        key_ = ResourceKey();
        definedByKey_ = false;
        setIcon( QIcon( new ResourceIconEngine<Resource>( resource_ ) ) );
}

template<class Resource>
void ResourceButton<Resource>::setResource( const ResourceKey& key ) 
{
        key_ = key;
        qDebug () << "setResource" << key;
        resource_ = resourceLib_[key];
        definedByKey_ = true;
        setIcon( QIcon( new ResourceIconEngine<Resource>( resource_ ) ) );
}


template<class Resource>
void ResourceButton<Resource>::changeResource()
{
        bool ok;
        
        if ( definedByKey_ ) { 
                ResourceKey newkey = ReslibEditor<Resource>::getResource( key_, &ok, this );
                if ( ok ) {
                        qDebug() << "ok, setting Key" << newkey;
                        setResource( newkey );
                        emit resourceChanged();
                }
                else qDebug() << "not ok";
                
        } else {
                Resource newRes = ResourceDialog<Resource>::editData( resource_, &ok, this );
                if ( ok ) {
                        setResource( newRes );
                        emit resourceChanged();
                }
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
