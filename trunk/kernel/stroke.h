 
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
#include "autohash.h"

#include <QColor>
#include <QBrush>

class Gradient;
class Pixmap;
class QPainterPath;
class QPainter;
class QRectF;

class OutputBackend;

//! Represents the possibilities of patterns that can used to draw or fill an object.
/*! So far it can contain a QColor or a Gradient. Pixmaps and vector
 *  patterns are to be implemented. The data is kept in an ResourceUser. 
 */
class Stroke 
{
public:
        //! the possible types of a Stroke
        enum StrokeType { sNone = 0, sColor, sGradient, sPixmap, sComplex };

        Stroke();
        explicit Stroke( const QColor& color );

        Stroke( const Stroke& other );

        ~Stroke();

        Stroke& operator= ( const Stroke& other );
                

        void setNone() { type_ = sNone; }
        void setColor( const QColor& color );

        void setColor( const ResourceKey& key );
        void setGradient( const ResourceKey& key );
        void setPixmap( const ResourceKey& key );

	bool setData( const QString& typeString, const ResourceKey& key );
        
        QColor color() const;
        bool isHardColor() const { return resourceUser_ && type_ == sColor; }

        StrokeType type() const { return type_; }
        
        const ResourceKey key() const;

        void fillPath( const QPainterPath& path, QPainter* painter ) const;
        
        const QBrush brush( const QRectF& rect ) const;
        const QBrush brush( const QPainterPath& path ) const;
        
        operator bool () const { return type_; }
        
        const QString typeString() const;

        AbstractResourceUser* resourceUser() const { return resourceUser_; }

        
private:
        StrokeType type_;

        AbstractResourceUser* resourceUser_;

	static const AutoHash<StrokeType, QString>& typeHash();
};

inline int qHash( const QColor& ) 
{
        return 0;
}


#endif
