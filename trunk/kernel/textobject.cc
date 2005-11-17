 
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
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>

TextObject::TextObject( Figure* parent )
        : DrawObject( parent ),
          text_(),
          textDocument_( this ),
          dummyPaintDevice_(),
          textLayout_( 0 ),
          font_(),
          alignment_( Qt::AlignHCenter | Qt::AlignVCenter ),
          cursor_( -1 ),
          cursorVisible_( false )
{
        textDocument_.documentLayout()->setPaintDevice( &dummyPaintDevice_ );
}


TextObject::TextObject( TextObject* o )
        : DrawObject( o ),
          text_( o->text_ ),
          textDocument_( this ),
          dummyPaintDevice_(),
          textLayout_( 0 ),
          font_( o->font_ ),
          alignment_( o->alignment_ ),
          cursor_( -1 ),
          cursorVisible_( false )
{
        textDocument_.documentLayout()->setPaintDevice( &dummyPaintDevice_ );
        getReadyForDraw();
}

TextObject::~TextObject()
{
        delete textLayout_;
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
//        painterPath_.addText( points_[0], font_, text_ );
}

void TextObject::setupRects()
{
        bRect_ = QRectF();
        
        textDocument_.setHtml( text_ );

        if ( !textLayout_ )
                textLayout_ = new QTextLayout( textDocument_.begin() );
        
        textLayout_->beginLayout();
        QTextLine line = textLayout_->createLine();
        textLayout_->endLayout();

        bRect_ = line.naturalTextRect();
        
        QPointF offset(0,0);
        
        if ( alignment_ & Qt::AlignRight )
                offset.rx() = bRect_.width();
        else if ( alignment_ & Qt::AlignHCenter )
                offset.rx() = bRect_.width()/2;

        if ( alignment_ & Qt::AlignBottom )
                offset.ry() = bRect_.height();
        if ( alignment_ & Qt::AlignVCenter )
                offset.ry() = bRect_.height()/2;
        
        actualPoint_ = points_[0] - offset;
        
        bRect_.translate( actualPoint_ );
        cRect_ = bRect_;
}

void TextObject::addPiece( const QString& piece )
{
        text_.append( piece );
        getReadyForDraw();
}

bool TextObject::pointHitsOutline( const QPointF& p, qreal tolerance ) const
{
        bRect_.contains( p );
}

void TextObject::draw( QPainter* p ) const
{
        if ( !textLayout_ )
                return;

        if ( !text_.isEmpty() )
                textLayout_->draw( p, actualPoint_ );
        if ( cursorVisible_ )
                textLayout_->drawCursor( p, actualPoint_, cursor_ );                
}

void TextObject::drawTentative( QPainter* p ) const
{
        draw( p );
}

void TextObject::setCursorPos( int c )
{
        cursor_ = c;
        if ( c < 0 )
                cursorVisible_ = false;
}

void TextObject::toggleCursor()
{
        cursorVisible_ = cursor_ >= 0 && !cursorVisible_;
}
