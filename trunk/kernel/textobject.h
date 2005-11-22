 
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

#include <QString>
#include <QFont>
#include <QTextDocument>
#include <QTextLayout>
#include <QPixmap>
#include <QList>

class TextObject : public DrawObject
{
        Q_OBJECT
public:
        TextObject( Figure* parent = 0 );
        TextObject( TextObject* o );
        ~TextObject();

        virtual DrawObject* copy();

        virtual const QString objectname() { return "text"; }

        QFont font() { return font_; }
        
        virtual void draw( QPainter* p ) const;
        virtual void drawTentative( QPainter* p ) const;
        
        virtual void outputToBackend( OutputBackend* ob );
        
        virtual bool pointHitsOutline( const QPointF& p, qreal tolerance ) const;

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
        
        void addPiece( int& pos, const QString& piece );
        void removePiece( int pos, int length );

        void insertByCursor( const QString& piece );
        void removeCharForward();
        void removeCharBackward();

        void incrementCursorPos();
        void decrementCursorPos();

        void setCursorPos( int c );
        void moveCursorToEnd();
        
        void toggleCursor();
        void hideCursor() { cursorVisible_ = false; }

//        QString& text() { return text_; }

public slots:
        void setFont( const QFont& f );

private:
        void setupPainterPath();
        void setupRects();
        void doSpecificPreparation() {}
        void doDraw( QPainter* p ) const;

        QPointF* nextPoint() { return 0; }

        void passPointFlag( Fig::PointFlag ) {}

        int realPos( int pos );

        QString text_;
        QTextDocument textDocument_;
        QPixmap dummyPaintDevice_;
        QTextLayout* textLayout_;
        QFont font_;
        QPointF actualPoint_;

        Qt::Alignment alignment_;

        int cursorPos_;
        int textLength_;
        bool cursorVisible_;

        QList<int> charLength_;
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
