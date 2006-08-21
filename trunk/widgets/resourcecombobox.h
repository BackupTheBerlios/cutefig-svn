 
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

#ifndef resourcecombobox_h
#define resourcecombobox_h

#include "reslib.h"
#include "resourceiconengine.h"

#include <QComboBox>

class AbstractResourceComboBox : public QComboBox
{
        Q_OBJECT
public:
        AbstractResourceComboBox( const ResourceKeyList keys, QWidget* parent );

        void setCurrentKey( const ResourceKey& key );
        ResourceKey currentKey() const;
        
signals:
        void activated( const ResourceKey& );
        void highlighted( const ResourceKey& );

protected:
        ResourceKeyList keys_;
        
private slots:
        void mapActivate( int i );
        void mapHighlight( int i );
};      


template<typename Resource>
class ResourceComboBox : public AbstractResourceComboBox
{
public:
        ResourceComboBox( QWidget* parent = 0 );

private:
        void init() {}
};

template<typename Resource>
ResourceComboBox<Resource>::ResourceComboBox( QWidget* parent )
        : AbstractResourceComboBox( ResLib<Resource>::instance().keys(), parent )
{
        ResLib<Resource>& rl = ResLib<Resource>::instance();
        foreach ( ResourceKey key , keys_ ) {
                AbstractResourceIconEngine* eng = IconEngineFactory<Resource>::orderEngine( rl[key] );
                if ( eng )
                        addItem( QIcon( eng ), QString() );
                else
                        addItem( key.keyString() );
        }

        init();
}


#endif
