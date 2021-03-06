 
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

#ifndef resourceiconengine_h
#define resourceiconengine_h

#include <QIconEngine>


class AbstractResourceIconEngine : public QIconEngine 
{
public:
        AbstractResourceIconEngine() : QIconEngine() {}
        ~AbstractResourceIconEngine() {}

        virtual void paint( QPainter* p, const QRect& r, QIcon::Mode md, QIcon::State );

protected:
        virtual void paintForeground( QPainter* p, const QRect& rect ) = 0;
};

template<typename Resource>
class ResourceIconEngine : public AbstractResourceIconEngine 
{
public:
        ResourceIconEngine<Resource>( const Resource& res )
                : AbstractResourceIconEngine(),
                  resource_( res )
        {
        }
        
        
        ~ResourceIconEngine() {}


protected:
        void paintForeground( QPainter* p, const QRect& rect );

        const Resource& resource_;
};

template<typename Resource>
class ResourceIconEngine<Resource*> : public AbstractResourceIconEngine
{
public:
        ResourceIconEngine<Resource*>( Resource* const res )
                : AbstractResourceIconEngine(),
                  resource_( res )
        {
        }
        
        
        ~ResourceIconEngine() {}


protected:
        void paintForeground( QPainter* p, const QRect& rect );

        Resource* const resource_;
};      


template<typename Resource>
class IconEngineFactory
{
public:
        static AbstractResourceIconEngine* orderEngine( const Resource& res ) 
        {
                return new ResourceIconEngine<Resource>( res );
        }
        
};

class Paper;
template<>
inline AbstractResourceIconEngine* IconEngineFactory<Paper>::orderEngine( const Paper& )
{
        return 0;
}

class Length;
template<>
inline AbstractResourceIconEngine* IconEngineFactory<Length>::orderEngine( const Length& ) 
{
        return 0;
}



class WhiteIcon;

template<>
class ResourceIconEngine<WhiteIcon> : public AbstractResourceIconEngine
{
public:
        void paintForeground( QPainter* p, const QRect& r );
};

#endif
