 
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
#include <QApplication>

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

        p->setPen( op );
}

void TextObject::drawTentative( QPainter* p ) const
{
        doDraw( p );
}

void TextObject::doDraw( QPainter* p ) const
{
        if ( !textLayout_ )
                return;

        QVector<QTextLayout::FormatRange> selection;

        if ( cursor_.hasSelection() ) {
                QTextLayout::FormatRange o;
                
                const QPalette& pal = QApplication::palette();
                o.start = cursor_.selectionStart();
                o.length = abs( cursor_.selectionEnd() - cursor_.selectionStart() );
                o.format.setBackground(pal.brush(QPalette::Highlight));
                o.format.setForeground(pal.brush(QPalette::HighlightedText));

                selection << o;
        }

        if ( !doc_.isEmpty() )
                textLayout_->draw( p, actualPoint_, selection );

        if ( cursorVisible_ && textLayout_ ) {
                p->setPen( QPen() );
                textLayout_->drawCursor( p, actualPoint_, cursor_.position() );
        }
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

//! A helper function that returns HTML tags to get from oldFormat to newFormat.
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

/*! We don't use QTextDocument::toHtml() as it's output is far to
 *  complicated, i.e. it is a complete HTML-document including
 *  <html><head></head><body></body></html> and stuff like that. It
 *  uses even CSS for the fonts and stuff.
 *
 *  So we create our HTML snippet ourselfes.
 *
 *  Therefore we take the plain text and replace critical characters
 *  by there entities first. Then we create a new QTextDocument on the
 *  stack. We don't use #doc_ here as we then could not qualify the
 *  method const and makeing #doc_ mutable would make me nervous.
 *
 *  We then use a QTextCursor to step through that temporary
 *  QTextDocument and to keep track of format changes.
 *
 *  Finally we replace newlines by <br>.
 */
const QString TextObject::text() const
{
        QString text = doc_.toPlainText();
        text.replace("&", "&amp;");
        text.replace("<", "&lt;");
        text.replace(">", "&gt;");

        int oldPosition = cursor_.position();
        cursor_.setPosition( 0 );
        QTextCharFormat currentFormat = cursor_.charFormat();
        QTextCharFormat lastFormat;

        int pos = 0;
        
        while ( !cursor_.atEnd() ) {
                cursor_.movePosition(QTextCursor::NextCharacter);
                currentFormat = cursor_.charFormat();
                QString tags = formatTags( lastFormat, currentFormat );
                if ( text[pos] == '&' )
                        while ( text[++pos] != ';');
                        
                text.insert( pos++, tags );
                pos += tags.length();
                lastFormat = currentFormat;
        }

        cursor_.setPosition( oldPosition );

        text.append( formatTags( currentFormat, QTextCharFormat() ) );

        text.replace("\n", "<br>");
        return text;
}

bool TextObject::isEmpty() const
{
        return doc_.isEmpty();
}

void TextObject::insertByCursor( const QString& piece )
{
        cursor_.insertText( piece, charFormat_ );
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

void TextObject::setCursorPos( int c, QTextCursor::MoveMode mode )
{
        cursor_.setPosition( c, mode );
        trackCharFormat();
}

int TextObject::cursorPos() const
{
        return cursor_.position();
}

void TextObject::incrementCursorPos( QTextCursor::MoveMode mode )
{
        cursor_.movePosition( QTextCursor::NextCharacter, mode );
        trackCharFormat();
}

void TextObject::decrementCursorPos( QTextCursor::MoveMode mode )
{
        cursor_.movePosition( QTextCursor::PreviousCharacter, mode );
        trackCharFormat();
}

void TextObject::moveCursorToEnd( QTextCursor::MoveMode mode )
{
        cursor_.movePosition( QTextCursor::End, mode );
        trackCharFormat();
}

void TextObject::setCursorToPoint( const QPointF& p )
{
        qreal x = p.x() - bRect_.left();
        //qreal y = p.y() - bRect_.top();

        QTextLine line = textLayout_->lineAt( 0 );
        setCursorPos( line.xToCursor( x ) );
}

bool TextObject::clearSelection()
{
        if ( !cursor_.hasSelection() )
                return false;
        
        cursor_.clearSelection();
        return true;
}

void TextObject::trackCharFormat()
{
        if ( charFormat_ != cursor_.charFormat() ) {
                charFormat_ = cursor_.charFormat();
                emit charFormatChanged();
        }
}

void TextObject::toggleBold()
{
        if ( charFormat_.font().bold() ) 
                charFormat_.setFontWeight( QFont::Normal );
        else 
                charFormat_.setFontWeight( QFont::Bold );

        cursor_.mergeCharFormat( charFormat_ );
}

void TextObject::toggleItalic()
{
        charFormat_.setFontItalic( !charFormat_.fontItalic() );
        cursor_.mergeCharFormat( charFormat_ );
}

void TextObject::toggleSuperScript()
{
        if ( charFormat_.verticalAlignment() == QTextCharFormat::AlignSuperScript )
                charFormat_.setVerticalAlignment( QTextCharFormat::AlignNormal );
        else
                charFormat_.setVerticalAlignment( QTextCharFormat::AlignSuperScript );

        cursor_.mergeCharFormat( charFormat_ );
        emit charFormatChanged();
}

void TextObject::toggleSubScript()
{
        if ( charFormat_.verticalAlignment() == QTextCharFormat::AlignSubScript )
                charFormat_.setVerticalAlignment( QTextCharFormat::AlignNormal );
        else
                charFormat_.setVerticalAlignment( QTextCharFormat::AlignSubScript );
        
        cursor_.mergeCharFormat( charFormat_ );
        emit charFormatChanged();
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

/*! Here the TextObject specific things are parsed. Note that only the
 *  font family and the font size are taken. Other font attributes,
 *  e.g bold, italic are encoded in HTML tags in the fontstring
 *  itself.
 */
template<>
DrawObject* TObjectHandler<TextObject>::parseObject( std::istream& is, Figure* fig )
{
        QString family;
        int pointSize;
        int alignment;
        QString text;

        is >> family >> pointSize >> alignment >> text;

        if ( is.fail() )
                return 0;

        TextObject* to = new TextObject( fig );
        to->setFont( QFont( family, pointSize ) );
        to->alignment_ = Qt::Alignment(alignment);
        to->setText( text );

        return to;
}

