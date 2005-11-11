 
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

#ifndef stroke_h
#define stroke_h

#include "resourcekey.h"
#include "resourceuser.h"
#include "gradient.h"

#include <QColor>
#include <QBrush>

class Gradient;
class QPixmap;
class QPainterPath;
class QPainter;
class QRectF;

class OutputBackend;

//template<typename Resource> class ResourceUser;

class Stroke 
{
public:
        enum StrokeType { sNone = 0, sColor, sGradient, sPixmap, sComplex };

        friend class OutputBackend;
        
        Stroke();
        explicit Stroke( const QColor& color );

//         static Stroke colorStroke( const ResourceKey& key );
//         static Stroke gradientStroke( const ResourceKey& key );
        
//        Stroke( const ResourceKey& key, const QColor& color );
//        Stroke( const ResourceKey& key, const Gradient& gradient );
        Stroke( const Stroke& other );

        ~Stroke();
        
        void setNone() { type_ = sNone; }
        void setColor( const QColor& color );
//        void setGradient( const Gradient& gradient );
        void setPixmap( const QPixmap& pixmap );

        void setColor( const ResourceKey& key );
        void setGradient( const ResourceKey& key );
        
        QColor color() const;
        bool isHardColor() const { return !key_.isValid() && type_ == sColor; }

        Gradient gradient() const;
        
        StrokeType type() const { return type_; }
        
//        void setKey( const ResourceKey& key );
        const ResourceKey& key() const { return key_; }

        void fillPath( const QPainterPath& path, QPainter* painter ) const;
        
        const QBrush brush( const QRectF& rect ) const;
        const QBrush brush( const QPainterPath& path ) const;
        
        operator bool () const { return type_; }
        
        const QString typeString() const;

        AbstractResourceUser* resourceUser() const { return resourceUser_; }

        Stroke& operator= ( const Stroke& other );
                
private:
        StrokeType type_;
//        QVariant data_;


        AbstractResourceUser* resourceUser_;

//        Gradient* gradient_;
        
        ResourceKey key_;
};

// inline Stroke Stroke::colorStroke( const ResourceKey& key ) 
// {
//         Stroke s;
//         s.type_ = sColor;
//         s.color_ = new ResourceUser<QColor>();
//         s.color_->setResource( key );
//         s.key_ = key;
        
//         return s;
// }

// inline Stroke Stroke::gradientStroke( const ResourceKey& key ) 
// {
//         Stroke s;
//         s.type_ = sGradient;
//         s.gradient_ = new ResourceUser<Gradient>();
//         s.gradient_->setResource( key );
//         s.key_ = key;
        
//         return s;
// }

inline int qHash( const QColor& ) 
{
        return 0;
}

inline int qHash( const QPixmap& ) 
{
        return 0;
}


#endif
