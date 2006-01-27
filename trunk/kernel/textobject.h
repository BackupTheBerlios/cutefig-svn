 
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

#ifndef textobject_h
#define textobject_h

#include "drawobject.h"
#include "objecthandler.h"

#include <QString>
#include <QFont>
#include <QTextDocument>
#include <QTextLayout>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QPixmap>
#include <QList>

//! The DrawObject representing text elements.
/*! A TextObject can also contain some rich text elements that are
 *  defined by HTML code. The TextObject class does not only define
 *  the TextObject itself but also an interface to manipiulate its
 *  contents. It therefore uses the QTextCursor infrastructure of Qt.
 *
 *  The textmanupulating stuff will eventually be packed into a
 *  different class... probably... maybe... erm... don't know.
 *
 *  The text rendering is done by Qt's rich text rendering facilities,
 *  i.e. QTextDocument, QTextLayout and friends. This is the reason
 *  why only font familie and font size of the font behind font() and
 *  setFont() are relevant.
 */
class TextObject : public DrawObject
{
        Q_OBJECT
        DRAW_OBJECT( "text", "&Text" );
public:
        explicit TextObject( Figure* parent = 0 );
        TextObject( const TextObject* o );
        ~TextObject();

        //! it is handy to make the parsing function friend.
        friend DrawObject* TObjectHandler<TextObject>::parseObject( std::istream&, Figure* fig );

        //! returns the font of the TextObject.
        /*! Note that only font family and size are relevant, as the
         *  rest is handled by the richt text facilities of Qt.
         */
        const QFont& font() const { return font_; }

        //! Only one point is necessary to determine the position of the TextObject.
        /*! The rest is determined by its size and #alignment_.
         */
        virtual uint minPoints() const { return 1; }
        
        virtual void draw( QPainter* p ) const;
        virtual void drawTentative( QPainter* p, const QPen& auxpen ) const;
        
        virtual void outputToBackend( OutputBackend* ob );
        
        virtual bool pointHitsOutline( const QPointF& p, qreal tolerance ) const;

        //! sets the contents, which is supposed to be a HTML snippet.
        void setText( const QString& text );
        
        void alignHCenter();
        bool isHCentered() const;

        void alignLeft();
        bool isLeftAligned() const;
        void alignRight();
        bool isRightAligned() const;

        void alignTop();
        bool isTopAligned() const;
        void alignVCenter();
        bool isVCentered() const;
        void alignBottom();
        bool isBottomAligned() const;

        void toggleBold();
        void toggleItalic();
        void toggleSuperScript();
        void toggleSubScript();
        
        Qt::Alignment alignment() const { return alignment_; }
        
        void insertByCursor( const QString& piece );
        void removeCharForward();
        void removeCharBackward();

        void incrementCursorPos( QTextCursor::MoveMode mode );
        void decrementCursorPos( QTextCursor::MoveMode mode );
        
        void moveCursorToEnd( QTextCursor::MoveMode mode );

        void setCursorPos( int c, QTextCursor::MoveMode mode = QTextCursor::MoveAnchor );
        int cursorPos() const;

        void setCursorToPoint( const QPointF& p );

        bool clearSelection();
        
        void toggleCursor();
        void hideCursor() { cursorVisible_ = false; }

        //! returns the contents of the TextObject as a HTML snippet.
        const QString text() const;
        bool isEmpty() const;

        const QTextCharFormat& charFormat() const { return charFormat_; }
        
signals:
        void charFormatChanged();
        
public slots:
        void setFont( const QFont& f );

private:
        void setupPainterPath() {}
        void setupRects();
        void doSpecificPreparation() {}
        void doDraw( QPainter* p ) const;

        int nextPointIndex() { return -1; }

        void passPointFlag( Fig::PointFlag ) {}

        void trackCharFormat();
        
        QTextDocument doc_;
        mutable QTextCursor cursor_;
        QTextCharFormat charFormat_;
        QPixmap dummyPaintDevice_;
        QTextLayout* textLayout_;
        QFont font_;
        QPointF actualPoint_;

        Qt::Alignment alignment_;

        bool cursorVisible_;
};

inline bool TextObject::isHCentered() const
{
        return alignment_ & Qt::AlignHCenter;
}

inline bool TextObject::isLeftAligned() const
{
        return alignment_ & Qt::AlignLeft;
}

inline bool TextObject::isRightAligned() const
{
        return alignment_ & Qt::AlignRight;
}

inline bool TextObject::isTopAligned() const
{
        return alignment_ & Qt::AlignTop;
}

inline bool TextObject::isVCentered() const
{
        return alignment_ & Qt::AlignVCenter;
}

inline bool TextObject::isBottomAligned() const
{
        return alignment_ & Qt::AlignBottom;
}



#endif
