 
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
#include "outputbackend.h"
#include "streamops.h"

#include <QRegExp>
#include <QFontMetricsF>
#include <QPainter>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>


TextObject::TextObject( Figure* parent )
        : DrawObject( parent ),
          doc_( this ),
          cursor_( &doc_ ),
          dummyPaintDevice_(),
          textLayout_( 0 ),
          font_(),
          alignment_( Qt::AlignVCenter ),
          cursorVisible_( false )
{
        doc_.documentLayout()->setPaintDevice( &dummyPaintDevice_ );
}


TextObject::TextObject( const TextObject* o )
        : DrawObject( o ),
          doc_( this ),
          cursor_( &doc_ ),
          dummyPaintDevice_(),
          textLayout_( 0 ),
          font_( o->font_ ),
          alignment_( o->alignment_ ),
          cursorVisible_( false )
{
        doc_.setHtml( o->doc_.toHtml() );
        doc_.documentLayout()->setPaintDevice( &dummyPaintDevice_ );
        getReadyForDraw();
}

TextObject::~TextObject()
{
        delete textLayout_;
}

void TextObject::outputToBackend( OutputBackend* ob )
{
        ob->outputTextObject( this );
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
                textLayout_->drawCursor( p, actualPoint_, cursor_.position() );

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

        if ( !doc_.isEmpty() )
                textLayout_->draw( p, actualPoint_ );    
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

void TextObject::setupRects()
{
        bRect_ = QRectF();
        
        doc_.setDefaultFont( font_ );

        if ( !textLayout_ )
                textLayout_ = new QTextLayout( doc_.begin() );

        textLayout_->setFont( font_ );
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

void TextObject::setText( const QString& text ) 
{
        doc_.setHtml( text );
}

QString formatTags( const QTextCharFormat& oldFormat, const QTextCharFormat& newFormat )
{
        QString tags;

        if ( !oldFormat.font().bold() && newFormat.font().bold() )
                tags += "<b>";
        else if ( oldFormat.font().bold() && !newFormat.font().bold() )
                tags += "</b>";

        if ( !oldFormat.fontItalic() && newFormat.fontItalic() )
                tags += "<i>";
        else if ( oldFormat.fontItalic() && !newFormat.fontItalic() )
                tags += "</i>";
        
        const QTextCharFormat::VerticalAlignment oldAl = oldFormat.verticalAlignment();
        const QTextCharFormat::VerticalAlignment newAl = newFormat.verticalAlignment();

        if ( !oldAl & QTextCharFormat::AlignSuperScript &&
             newAl & QTextCharFormat::AlignSuperScript )
                tags += "<sup>";
        else if ( oldAl & QTextCharFormat::AlignSuperScript &&
                  !newAl & QTextCharFormat::AlignSuperScript )
                tags += "</sup>";
        
        if ( !oldAl & QTextCharFormat::AlignSubScript &&
             newAl & QTextCharFormat::AlignSubScript )
                tags += "<sub>";
        else if ( oldAl & QTextCharFormat::AlignSubScript &&
                  !newAl & QTextCharFormat::AlignSubScript )
                tags += "</sub>";

        return tags;
}


const QString TextObject::text() const
{
        QString text = doc_.toPlainText();

        QTextDocument d( doc_.toHtml() );
        QTextCursor crs( &d );
        QTextCharFormat currentFormat = crs.charFormat();
        QTextCharFormat lastFormat;

        int pos = 0;
        
        while ( !crs.atEnd() ) {
                crs.movePosition(QTextCursor::NextCharacter);
                currentFormat = crs.charFormat();
                QString tags = formatTags( lastFormat, currentFormat );
                text.insert( pos++, tags );
                pos += tags.length();
                lastFormat = currentFormat;
        }

        text.append( formatTags( currentFormat, QTextCharFormat() ) );

        text.replace("\n", "<br>");
        text.replace("&", "&amp;");
        text.replace("<", "&lt;");
        text.replace(">", "&gt;");
        
        return text;
}

void TextObject::insertByCursor( const QString& piece )
{
        qDebug() << "insertByCursor" << cursor_.position() << piece << charFormat_.fontWeight();
        cursor_.insertText( piece, charFormat_ );
        qDebug() << doc_.toPlainText();
        getReadyForDraw();
}

void TextObject::removeCharForward()
{
        cursor_.deleteChar();
        trackCharFormat();
        getReadyForDraw();
}

void TextObject::removeCharBackward()
{
        cursor_.deletePreviousChar();
        trackCharFormat();
        getReadyForDraw();
}

void TextObject::setCursorPos( int c )
{
        cursor_.setPosition( c );
        trackCharFormat();
}

int TextObject::cursorPos() const
{
        return cursor_.position();
}

void TextObject::incrementCursorPos()
{
        cursor_.movePosition( QTextCursor::NextCharacter );
        trackCharFormat();
}

void TextObject::decrementCursorPos()
{
        cursor_.movePosition( QTextCursor::PreviousCharacter );
        trackCharFormat();
}

void TextObject::moveCursorToEnd()
{
        cursor_.movePosition( QTextCursor::End );
        trackCharFormat();
}

void TextObject::trackCharFormat()
{
        if ( charFormat_ != cursor_.charFormat() ) {
                qDebug () << "charFormat_ changed";
                charFormat_ = cursor_.charFormat();
                emit charFormatChanged();
        }
}

bool TextObject::isBold() const
{
        return cursor_.charFormat().fontWeight() > QFont::Normal;
}

bool TextObject::toggleBold()
{
        qDebug() << "toggleBold";
        if ( isBold() ) {
                charFormat_.setFontWeight( QFont::Normal );
                return false;
        } else {
                charFormat_.setFontWeight( QFont::Bold );
                return true;
        }
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

template<>
DrawObject* TObjectHandler<TextObject>::parseObject( std::istream& is, Figure* fig )
{
        QString family;
        int pointSize;
        bool italic;
        int weight, alignment;
        QString text;

        is >> family >> pointSize >> italic >> weight >> alignment >> text;

        if ( is.fail() )
                return 0;

        TextObject* to = new TextObject( fig );
        to->setFont( QFont( family, pointSize, weight, italic ) );
        to->alignment_ = Qt::Alignment(alignment);
        to->setText( text );

        return to;
}
