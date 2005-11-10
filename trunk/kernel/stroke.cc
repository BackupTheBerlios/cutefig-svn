 
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
          color_( 0 ),
          gradient_( 0 ),
          pixmap_( 0 ),
          key_()
{}

Stroke::Stroke( const Stroke& other )
        : type_( other.type_ ),
          color_( other.color_ ? new ResourceUser<QColor>( *other.color_ ) : 0 ),
          gradient_( other.gradient_ ? new ResourceUser<Gradient>( *other.gradient_ ) : 0 ),
          pixmap_( other.pixmap_ ? new ResourceUser<QPixmap>( *other.pixmap_ ) : 0  ),
          key_( other.key_ )
{
}

Stroke::Stroke( const QColor& color )
        : type_( sColor ),
          color_( new ResourceUser<QColor>( color ) ),
          gradient_( 0 ),
          pixmap_( 0 ),
          key_()
{}

Stroke& Stroke::operator=( const Stroke& other )
{
        type_ = other.type_;

        if ( color_ )
                delete color_;
        
        color_ = other.color_ ? new ResourceUser<QColor>( *other.color_ ) : 0;

        if ( gradient_ )
                delete gradient_;
        
        gradient_ = other.gradient_ ? new ResourceUser<Gradient>( *other.gradient_ ) : 0;

        if ( pixmap_ )
                delete pixmap_;
        
        pixmap_ = other.pixmap_ ? new ResourceUser<QPixmap>( *other.pixmap_ ) : 0;
        key_ = other.key_;

        return *this;
}


Stroke::~Stroke()
{
        if ( color_ )
                delete color_;
        
        if ( gradient_ ) 
                delete gradient_;        

        if ( pixmap_ ) 
                delete pixmap_;
}

// Stroke::Stroke( const ResourceKey& key, const QColor& color )
//         : type_( sColor ),
//           color_( new ResourceUser<QColor>( color ) ),
//           gradient_( 0 ),
//           key_()
// {
//         if ( key.isValid() )
//                 key_ = key;
// }

// Stroke::Stroke( const ResourceKey& key, const Gradient& gradient )
//         : type_(),
//           key_()
// {
//         if ( key.isValid() ) {
//                 key_ = key;
//                 type_ = sGradient;
//                 data_.setValue( gradient );
//         }
// }

void Stroke::setColor( const QColor& color )
{
        type_ = sColor;

        if ( gradient_ ) {
                delete gradient_;        
                gradient_ = 0;
        }

        if ( pixmap_ ) {
                delete pixmap_;
                pixmap_ = 0;
        }
        
        if ( !color_ )
                color_ = new ResourceUser<QColor>();
        
        color_->setResource( color );
}

// void Stroke::setGradient( const Gradient& gradient )
// {
//         qDebug() << "*** Bug *** Stroke::setGradient() without key...";

//         type_ = sGradient;

//         if ( color_ ) {
//                 delete color_;
//                 color_ = 0;
//         }

//         if ( pixmap_ ) {
//                 delete pixmap_;
//                 pixmap_ = 0;
//         }
        
//         if ( !gradient_ )
//                 gradient_ = new ResourceUser<Gradient>();
        
//         gradient_->setResource( gradient );
// }

void Stroke::setColor( const ResourceKey& key )
{
        type_ = sColor;
        
        if ( gradient_ ) {
                delete gradient_;
                gradient_ = 0;
        }

        if ( pixmap_ ) {
                delete pixmap_;        
                pixmap_ = 0;
        }

        if ( !color_ )
                color_ = new ResourceUser<QColor>();

        color_->setResource( key );
        key_ = key;
}

void Stroke::setGradient( const ResourceKey& key )
{
        type_ = sGradient;
        
        if ( color_ ) {
                delete color_;
                color_ = 0;
        }

        if ( pixmap_ ) {
                delete pixmap_;        
                pixmap_ = 0;
        }

        if ( gradient_ ) {
                delete gradient_;
                gradient_ = 0;
        }
                        
        gradient_ = new ResourceUser<Gradient>();

        gradient_->setResource( key );
        key_ = key;
}

void Stroke::setPixmap( const QPixmap& pixmap )
{
        qDebug() << "*** Bug *** Stroke::setPixmap() without key...";

        type_ = sPixmap;
        
        if ( color_ ) {
                delete color_;
                color_ = 0;
        }

        if ( gradient_ ) {
                delete gradient_;        
                gradient_ = 0;
        }
        
        if ( !pixmap_ )
                pixmap_ = new ResourceUser<QPixmap>();
        
        pixmap_->setResource( pixmap );
}

QColor Stroke::color() const
{
        if ( type_ == sColor )
                return color_->data();
        else
                return QColor();
}

Gradient Stroke::gradient() const
{
        if ( type_ == sGradient )
                return gradient_->data();
        else
                return Gradient();
}

void Stroke::fillPath( const QPainterPath& path, QPainter* painter ) const
{
        switch ( type_ ) {
            case sColor:
                    painter->fillPath( path, QBrush( color_->data() ) );
                    break;
            case sGradient:
            {
                    QRectF r = path.boundingRect();
                    QGradient* qgrad = gradient_->data().toQGradient( r );
                    if ( qgrad )
                            painter->fillPath( path, *qgrad );
                    delete qgrad;
                    break;
            }
            case sPixmap:
                    painter->fillPath( path, QBrush( pixmap_->data() ) );
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
                    ret = color_->data();
                    break;
            case sGradient:
            {
                    QGradient* qgrad = gradient_->data().toQGradient( rect );
                    ret = *qgrad;
                    break;
            }
            case sPixmap:
                    ret = pixmap_->data();
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


void Stroke::addUsedResource( ResourceSet& rs ) const
{
        if ( color_ ) {
                rs["color"] << color_->key();
                return;
        }
        if ( gradient_ ) {
                rs["gradient"] << gradient_->key();
                return;
        }
}
