 
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

#ifndef pen_h
#define pen_h

#include <QColor>
#include <QVector>
#include <QRectF>

#include "typedefs.h"
#include "resourcekey.h"

class QPainterPath;
class QPainter;

class Stroke;

class Pen
{
public:
        Pen();
        Pen( const Pen& p );

        ~Pen() {}

        void setWidth( qreal lw ) { lineWidth_ = lw; }
        qreal width() const { return lineWidth_; }

        void setCapStyle( Qt::PenCapStyle cs ) { capStyle_ = cs; }
        Qt::PenCapStyle capStyle() const { return capStyle_; }

        void setJoinStyle( Qt::PenJoinStyle js ) { joinStyle_ = js; }
        Qt::PenJoinStyle joinStyle() const { return joinStyle_; }

        bool setDashes( const ResourceKey& key );
        const ResourceKey& dashesKey() const { return dashesKey_; }

        void strikePath( const QPainterPath& path, const Stroke&, QPainter* painter ) const;
        
        QRectF pathRect( const QPainterPath& path ) const;

private:
        void setupPainterPath( QPainterPath& strokePath, const QPainterPath& path ) const;
        
        qreal lineWidth_;

        Qt::PenCapStyle capStyle_;
        Qt::PenJoinStyle joinStyle_;

        ResourceKey dashesKey_;
        Dashes dashes_;
};

#endif

