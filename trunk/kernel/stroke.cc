 
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


#include "stroke.h"
#include "gradient.h"
#include "resourceuser.h"

#include <QPainter>
#include <QPainterPath>
#include <QColor>
#include <QBrush>

#include <QDebug>

Stroke::Stroke()
        : type_( sNone ),
          resourceUser_( 0 )
{}

Stroke::Stroke( const Stroke& other )
        : type_( other.type_ ),
          resourceUser_( other.resourceUser_ ? other.resourceUser_->clone() : 0 )
{
}

Stroke::Stroke( const QColor& color )
        : type_( sColor ),
          resourceUser_( new ResourceUser<QColor>( color ) )
{}

Stroke& Stroke::operator=( const Stroke& other )
{
        if ( this == &other )
                return *this;
        
        type_ = other.type_;

        delete resourceUser_;
        
        resourceUser_ = other.resourceUser_ ? other.resourceUser_->clone() : 0;
        
        return *this;
}


Stroke::~Stroke()
{
        delete resourceUser_;
}

void Stroke::setColor( const QColor& color )
{
        type_ = sColor;

        delete resourceUser_;
        
        resourceUser_ = new ResourceUser<QColor>();
        static_cast<ResourceUser<QColor>*>( resourceUser_ )->setResource( color );
}

void Stroke::setColor( const ResourceKey& key )
{
        type_ = sColor;

        delete resourceUser_;
        
        resourceUser_ = new ResourceUser<QColor>();
        static_cast<ResourceUser<QColor>*>( resourceUser_ )->setResource( key );
}

void Stroke::setGradient( const ResourceKey& key )
{
        type_ = sGradient;

        delete resourceUser_;
        
        resourceUser_ = new ResourceUser<Gradient>();
        static_cast<ResourceUser<Gradient>*>( resourceUser_ )->setResource( key );
}

void Stroke::setPixmap( const QPixmap& pixmap )
{
        type_ = sPixmap;
        
        delete resourceUser_;
        
        if ( !resourceUser_ )
                resourceUser_ = new ResourceUser<QPixmap>();
        
        static_cast<ResourceUser<QPixmap>*>( resourceUser_ )->setResource( pixmap );
}

QColor Stroke::color() const
{
        if ( type_ == sColor )
                return resourceUser_->resource<QColor>();
        else
                return QColor();
}


void Stroke::fillPath( const QPainterPath& path, QPainter* painter ) const
{
        switch ( type_ ) {
            case sColor:
                    painter->fillPath( path, QBrush( resourceUser_->resource<QColor>() ) );
                    break;
            case sGradient:
            {
                    QRectF r = path.boundingRect();
                    QGradient* qgrad = resourceUser_->resource<Gradient>().toQGradient( r );
                    if ( qgrad )
                            painter->fillPath( path, *qgrad );
                    delete qgrad;
                    break;
            }
            case sPixmap:
                    painter->fillPath( path, QBrush( resourceUser_->resource<QPixmap>() ) );
                    break;
            case sNone:
            default: break;
        }
}

const QBrush Stroke::brush( const QRectF& rect ) const
{
        QBrush ret;
        
        switch ( type_ ) {
            case sColor:
                    ret = resourceUser_->resource<QColor>();
                    break;
            case sGradient:
            {
                    QGradient* qgrad = resourceUser_->resource<Gradient>().toQGradient( rect );
                    if ( qgrad )
                            ret = QBrush( *qgrad );
                    delete qgrad;
                    break;
            }
            case sPixmap:
                    ret = resourceUser_->resource<QPixmap>();
                    break;
            case sNone:
            default: break;
        }

        return ret;    
}

const QBrush Stroke::brush( const QPainterPath& path ) const
{
        QRectF r;

        if ( type_ == sGradient )
                r = path.boundingRect();

        return brush( r );
}

const QString Stroke::typeString() const
{
        switch ( type_ ) {
            case sColor:    return "color";
            case sGradient: return "gradient";
                    
            default: break;
        } 

        return QString();
}


const ResourceKey Stroke::key() const
{
        return resourceUser_ ? resourceUser_->key() : ResourceKey();
}
