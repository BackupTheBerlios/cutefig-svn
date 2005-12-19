 
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
#include "resourceuser.h"

class QPainterPath;
class QPainter;

class Stroke;

//! The pen style "on top of" Stroke
/*! Pen defines the linestyle (dashpattern) of a DrawObjects
 *  outline. So far it uses Qt's QPainterPathStroker to get abitrary
 *  dash patterns. Possibly we will swich back to simple QPen stuff as
 *  QPen now supports abitrary dashpatterns out of the box.
 *
 *  The class Pen will be kept though as it owns the ResourceUser for
 *  Dashes.
 */
class Pen
{
public:
        Pen();
        Pen( const Pen& p );

        Pen& operator=( const Pen& other );
        
        ~Pen();

        //! sets the line width
        void setWidth( qreal lw ) { lineWidth_ = lw; }
        //! returns the line width
        qreal width() const { return lineWidth_; }

        //! sets the cap style
        void setCapStyle( Qt::PenCapStyle cs ) { capStyle_ = cs; }
        //! returns the cap style
        Qt::PenCapStyle capStyle() const { return capStyle_; }

        //! sets the join style
        void setJoinStyle( Qt::PenJoinStyle js ) { joinStyle_ = js; }
        //! returns the join style
        Qt::PenJoinStyle joinStyle() const { return joinStyle_; }

        //! sets the dash pattern using ResLib<Dashes>::instance()
        void setDashes( const ResourceKey& key );
        //! returns the ResourceKey of the used dash pattern
        const ResourceKey& dashesKey() const { return dashes_->key(); }

        //! paints the outline of path on the painter using the given Stroke
        void strikePath( const QPainterPath& path, const Stroke&, QPainter* painter ) const;

        //! returns the bounding rect of the path when stroken with painted using strikePath()
        QRectF pathRect( const QPainterPath& path ) const;

        //! returns the ResourceUser using the dashes
        AbstractResourceUser* resourceUser() const { return dashes_; }
        
private:
        //! prepares the painter path
        void setupPainterPath( QPainterPath& strokePath, const QPainterPath& path ) const;
        
        qreal lineWidth_;

        Qt::PenCapStyle capStyle_;
        Qt::PenJoinStyle joinStyle_;

        ResourceUser<Dashes>* dashes_;
};

int qHash( const Dashes& dashes );

#endif

