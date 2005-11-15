 
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


#include "textobject.h"

#include <QFontMetricsF>
#include <QPainter>

TextObject::TextObject( Figure* parent )
        : DrawObject( parent ),
          text_(),
          font_()
{}


TextObject::TextObject( TextObject* o )
        : DrawObject( o ),
          text_( o->text_ ),
          font_( o->font_ )
{
        getReadyForDraw();
}

DrawObject* TextObject::copy()
{
        return new TextObject( this );
}

void TextObject::outputToBackend( OutputBackend* ob )
{
    
}

void TextObject::setupPainterPath()
{
        painterPath_.addText( points_[0], font_, text_ );
}

void TextObject::setupRects()
{
        bRect_ = QFontMetricsF( font_ ).boundingRect( text_ );
}

void TextObject::addPiece( const QString& piece )
{
        text_.append( piece );
        getReadyForDraw();
}

bool TextObject::pointHitsOutline( const QPointF& p, qreal tolerance ) const
{
        return false;
}

void TextObject::draw( QPainter* p ) const
{
        p->setFont( font_ );
        p->drawText( points_[0], text_ );
}
