#ifndef arrow_h
#define arrow_h
 
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

#include "typedefs.h"
#include "resourceuser.h"

#include <QString>

class QPainter;
class QPointF;

//! A class to draw different types of arrows.
/*! In order to draw different types of arrows in a extensible way we
 *  need a class that does the drawing polymorphically. Therefore
 *  ArrowPainter is the abstract base class. The only function that is
 *  relevant is draw().
 */
class ArrowPainter
{
public:
        virtual ~ArrowPainter() {}

        //! Does the actual drawing on the QPainter p.
        /*! \param pos is the <e>hot spot</e> of the arrow. \param
         *  represents the direction the arrow is pointing to as a
         *  vector. Those parameters are passed on by the caller of
         *  Arrow::draw(). Where as the parameters \param w and \param
         *  l, representing width and height of the Arrow are coming
         *  from data members of Arrow.
         */
        virtual void draw( const QPointF& pos, const QPointF& angle, QPainter* p,
                           double w, double l ) const = 0;
};


//! A class that represents arrows at the end of lines.
/*! The relevant data members are the width w and the length l of the
 *  Arrow. The way the arrow is drawn is determined by an
 *  ArrowPainter. The ArrowPainter is stored in a ResLib as
 *  pointers. In an Arrow the ArrowPainter is held in an ResourceUser.
 */
class Arrow
{
public:
        Arrow( ArrowPainter* ap = 0, double l = 10., double w = 5. );
        Arrow( const ResourceKey& k, double w, double l );
        
        ~Arrow() {}

        Arrow( const Arrow& other )
                : arrowPainter_( other.arrowPainter_ ),
                  width_( other.width_ ),
                  length_( other.length_ )
        {}

        Arrow& operator= ( const Arrow& other );

        bool isValid() const;

        double width() const { return width_; }
        double length() const { return length_; }

        void setWidth( double w ) { width_ = w; }
        void setLength( double l ) { length_ = l; }

        void setArrowPainter( const ResourceKey& k ) { arrowPainter_.setResource( k ); }

        const ResourceKey& key() const { return arrowPainter_.key(); }

        void draw( const QPointF& pos, const QPointF& angle, QPainter* p ) const;
        
private:
        ResourceUser<ArrowPainter*> arrowPainter_;

        double width_, length_;
};

//uint qHash( const Arrow& a );

namespace Res
{
        template<typename Resource> const QString resourceName();
        template<> inline const QString resourceName<ArrowPainter*>() { return "arrow"; }
}

#endif
