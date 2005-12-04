 
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
#include "stroke.h"

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
          alignment_( Qt::AlignVCenter ),
          cursorPos_( 0 ),
          textLength_( 0 ),
          cursorVisible_( false ),
          charLength_()
{
        textDocument_.documentLayout()->setPaintDevice( &dummyPaintDevice_ );
}


TextObject::TextObject( const TextObject* o )
        : DrawObject( o ),
          text_( o->text_ ),
          textDocument_( this ),
          dummyPaintDevice_(),
          textLayout_( 0 ),
          font_( o->font_ ),
          alignment_( o->alignment_ ),
          cursorPos_( 0 ),
          textLength_( 0 ),
          cursorVisible_( false ),
          charLength_( o->charLength_ )
{
        textDocument_.documentLayout()->setPaintDevice( &dummyPaintDevice_ );
        getReadyForDraw();
}

TextObject::~TextObject()
{
        delete textLayout_;
}

void TextObject::outputToBackend( OutputBackend* )
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

        textDocument_.setDefaultFont( font_ );

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


void TextObject::addPiece( int& pos, const QString& p )
{
        QString st;
        int realStart = realPos( pos );
        for ( int i=0; i < p.length(); ++i ) {
                if ( p.at(i) == '<' ) {
                        st += "&lt;";
                        charLength_.insert( pos, 4 );
                }
                else if ( p.at(i) == '>' ) {
                        st += "&gt;";
                        charLength_.insert( pos, 4 );
                }
                else if ( p.at(i) == '&' ) {
                        st += "&amp;";
                        charLength_.insert( pos, 5 );
                }
                else {
                        st += p.at( i );
                        charLength_.insert( pos, 1 );
                }
                
                ++pos;
                ++textLength_;
        }
        
        text_.insert( realStart, st ); 

        getReadyForDraw();
}

void TextObject::removePiece( int pos, int length )
{
        for ( int i = 0; i < length; ++i ) {
                text_.remove( realPos( pos ), charLength_[pos] );
                charLength_.removeAt( pos );
                --textLength_;
        }

        getReadyForDraw();
}

void TextObject::insertByCursor( const QString& piece )
{
        addPiece( cursorPos_, piece );
}

void TextObject::removeCharForward()
{
        if ( cursorPos_ < textLength_ )
                removePiece( cursorPos_, 1 );
}

void TextObject::removeCharBackward()
{
        if ( cursorPos_ )
                removePiece( --cursorPos_, 1 );
}

int TextObject::realPos( int pos )
{
        int ret = 0;
        
        for ( int i = 0; i < pos; ++i )
                ret += charLength_[i];

        return ret;
}

bool TextObject::pointHitsOutline( const QPointF& p, qreal ) const
{
        return bRect_.contains( p );
}

void TextObject::draw( QPainter* p ) const
{
        QPen op = p->pen();
        p->setPen( QPen( stroke_.brush( bRect_ ), 0 ) );

        doDraw( p );
        if ( cursorVisible_ && textLayout_ )
                textLayout_->drawCursor( p, actualPoint_, cursorPos_ );

        p->setPen( op );
}

void TextObject::drawTentative( QPainter* p ) const
{
        draw( p );
}

void TextObject::doDraw( QPainter* p ) const
{
        if ( !textLayout_ )
                return;

        if ( !text_.isEmpty() )
                textLayout_->draw( p, actualPoint_ );    
}

void TextObject::setCursorPos( int c )
{
        cursorPos_ = c;
        if ( c < 0 )
                cursorVisible_ = false;
}

void TextObject::toggleCursor()
{
        cursorVisible_ = !cursorVisible_;
}

void TextObject::setFont( const QFont& f )
{
        font_ = f;
        getReadyForDraw();
}


void TextObject::incrementCursorPos()
{
        if ( realPos( cursorPos_ ) < text_.length() )
                ++cursorPos_;
}

void TextObject::decrementCursorPos()
{
        if ( cursorPos_ )
                --cursorPos_;
}

void TextObject::moveCursorToEnd()
{
        setCursorPos( textLength_ );
}

void TextObject::alignHCenter()
{
        if ( isHCentered() )
                return;
        
        alignment_ &= Qt::AlignVertical_Mask;
        alignment_ |= Qt::AlignHCenter;

        getReadyForDraw();
}

void TextObject::alignLeft()
{
        if ( isLeftAligned() )
                return;
        
        alignment_ &= Qt::AlignVertical_Mask;
        alignment_ |= Qt::AlignLeft;

        getReadyForDraw();
}


void TextObject::alignRight()
{
        if ( isRightAligned() )
                return;

        alignment_ &= Qt::AlignVertical_Mask;
        alignment_ |= Qt::AlignRight;

        getReadyForDraw();
}

void TextObject::alignTop()
{
        if ( isTopAligned() )
                return;

        alignment_ &= Qt::AlignHorizontal_Mask;
        alignment_ |= Qt::AlignTop;

        getReadyForDraw();
}

void TextObject::alignVCenter()
{
        if ( isVCentered() )
                return;

        alignment_ &= Qt::AlignHorizontal_Mask;
        alignment_ |= Qt::AlignVCenter;

        getReadyForDraw();
}

void TextObject::alignBottom()
{
        if ( isBottomAligned() )
                return;

        alignment_ &= Qt::AlignHorizontal_Mask;
        alignment_ |= Qt::AlignBottom;

        getReadyForDraw();
}
