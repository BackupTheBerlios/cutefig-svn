 
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

class Gradient;
class QPixmap;
class QPainterPath;
class QPainter;

class OutputBackend;

#include "resourcekey.h"

#include <QVariant>
#include <QColor>

class Stroke 
{
public:
        enum StrokeType { sNone = 0, sColor, sGradient, sPixmap, sComplex };

        friend class OutputBackend;
        
        Stroke();
        Stroke( const QColor& color );
        Stroke( Gradient* gradient );
        Stroke( const Stroke& other );

        ~Stroke() {}

        void setNone() { type_ = sNone; }
        void setColor( const QColor& color );
        void setGradient( Gradient* gradient );
        void setPixmap( const QPixmap& pixmap );

        QColor color() const;
        bool isHardColor() const { return !key_.isValid() && type_ == sColor; }

        Gradient* gradient() const { return gradient_; }
        
        StrokeType type() const { return type_; }
        
        void setKey( const ResourceKey& key ) { key_ = key; }
        const ResourceKey& key() const { return key_; }

        void fillPath( const QPainterPath& path, QPainter* painter ) const;
        const QBrush brush( const QPainterPath& path ) const;
        
        operator bool () const { return type_; }
        
        
private:
        StrokeType type_;
        QVariant data_;
        
        Gradient* gradient_;
        
        ResourceKey key_;
};

#endif
