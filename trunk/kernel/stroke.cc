 
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

#include <QPainter>
#include <QPainterPath>
#include <QColor>
#include <QBrush>

#include <QDebug>

Stroke::Stroke()
        : type_( sNone ),
          data_( QVariant() ),
          key_()
{
}

Stroke::Stroke( const Stroke& other )
        : type_( other.type_ ),
          data_( other.data_ ),
          key_( other.key_ )
{
}

Stroke::Stroke( const QColor& color )
        : type_( sColor ),
          data_( QVariant( color ) ),
          key_()
{
}

Stroke::Stroke( const ResourceKey& key, const QColor& color )
        : type_( sColor ),
          data_( QVariant( color ) ),
          key_()
{
        if ( key.isValid() )
                key_ = key;
}

Stroke::Stroke( const ResourceKey& key, const Gradient& gradient )
        : type_(),
          key_()
{
        if ( key.isValid() ) {
                key_ = key;
                type_ = sGradient;
                data_.setValue( gradient );
        }
}

void Stroke::setColor( const QColor& color )
{
        type_ = sColor;
        data_ = QVariant( color );
}

void Stroke::setGradient( const Gradient& gradient )
{
        if ( key_.isValid() ) {
                type_ = sGradient;
                data_.setValue( gradient );
        } else
                qDebug() << "*** Bug *** Stroke::setGradient() without key...";
}

void Stroke::setPixmap( const QPixmap& pixmap )
{
        if ( key_.isValid() ) {
                type_ = sPixmap;
                data_.setValue( pixmap );
        } else
                qDebug() << "*** Bug *** Stroke::setPixmap() without key...";
}

QColor Stroke::color() const
{
        if ( type_ == sColor )
                return data_.value<QColor>();
        else
                return QColor();
}

Gradient Stroke::gradient() const
{
        if ( type_ == sGradient )
                return data_.value<Gradient>();
        else
                return Gradient();
}

void Stroke::fillPath( const QPainterPath& path, QPainter* painter ) const
{
        switch ( type_ ) {
            case sColor:
                    painter->fillPath( path, QBrush( data_.value<QColor>() ) );
                    break;
            case sGradient:
            {
                    QRectF r = path.boundingRect();
                    QGradient* qgrad = data_.value<Gradient>().toQGradient( r );
                    painter->fillPath( path, *qgrad );
                    delete qgrad;
                    break;
            }
            case sPixmap:
                    painter->fillPath( path, QBrush( data_.value<QPixmap>() ) );
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
                    ret = data_.value<QColor>();
                    break;
            case sGradient:
            {
                    QGradient* qgrad = data_.value<Gradient>().toQGradient( rect );
                    ret = *qgrad;
                    break;
            }
            case sPixmap:
                    ret = data_.value<QPixmap>();
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
