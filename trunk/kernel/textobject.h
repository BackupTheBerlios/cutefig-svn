 
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


class TextObject : public DrawObject
{
        Q_OBJECT
public:
        TextObject( Figure* parent = 0 );
        TextObject( TextObject* o );
        ~TextObject();

        virtual DrawObject* copy();

        virtual const QString objectname() { return "text"; }

        virtual void draw( QPainter* p ) const;
        
        virtual void outputToBackend( OutputBackend* ob );
        
        virtual bool pointHitsOutline( const QPointF& p, qreal tolerance ) const;

        void addPiece( const QString& piece );
        void update() { getReadyForDraw(); }

        QString& text() { return text_; }
        
private:
        void setupPainterPath();
        void setupRects();
        void doSpecificPreparation() {}

        QPointF* nextPoint() { return 0; }

        void passPointFlag( Fig::PointFlag f ) {}

        QString text_;
        QTextDocument textDocument_;
        QPixmap dummyPaintDevice_;
        QTextLayout* textLayout_;
        QFont font_;
        QPointF actualPoint_;

        Qt::Alignment alignment_;
};


#endif
