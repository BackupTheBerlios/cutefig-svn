 
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

#ifndef parser_h
#define parser_h

/** \class Parser
 *  \brief Parses a input file.
 */
#include <QObject>
#include <QString>
#include <QPolygonF>
#include <QPen>
#include <QHash>

#include <sstream>

#include "typedefs.h"
#include "pen.h"
#include "stroke.h"

class QTextStream;
class DrawObject;
class Figure;



enum ErrorSeverity { Warning, Discarding, Fatal };

class Parser : public QObject
{
        Q_OBJECT
public:
        Parser( QTextStream *ts, Figure *f, 
                QObject * parent =0 );
        ~Parser() { };

        QString parse();

        static Dashes parseDashLine( const std::string& s );
        
private:
        ObjectList parseLoop( bool parsingCompound = false );
        QPolygonF getPoints( uint n );
        bool readLine();

        typedef DrawObject* (Parser::*creatorType)();
        QHash<QString, creatorType> registeredTypes_;
        
        QTextStream *fileStream_;
        std::istringstream stream_;
        
        QString itemType_;
        uint line_;
        Figure* figure_;
        
        QString errorReport_;

        QString objectComment_;

        DrawObject *createEllipse();
        DrawObject *createPolyline();
        DrawObject *createPolygon();
        
        DrawObject *createObject( QString name );
        DrawObject *parseGenericData( uint &npoints, QPolygonF*& pa );
        QPointF parsePoint();

        void pushDashes();
        void pushColor();
        void pushGradient();
        
        ResourceKeyList dashList_;
        
//        QBrush parsePattern( int lines );

//        Dashes parseDashes( std::istringstream& is );
        
        QPen setUpPen( int w, int s, int cs, int js, QColor c );
        void parseError( QString s, ErrorSeverity sev = Warning );

        static const QString unknownItemType;        
        static const QString unknownObject;
        static const QString invalidStandardLine;
        static const QString invalidObjectData;
        static const QString notEnoughPoints;
        static const QString wrongPointNumber;
        static const QString ignoringPoint;
        static const QString invalidPenStyle;
        static const QString invalidCapStyle;
        static const QString invalidJoinStyle;
        static const QString invalidPoint;
        static const QString invalidColor;
        static const QString invalidDashLine;
        static const QString undefinedDashes;
        static const QString compound_end_without_compound;
        static const QString unexpectedEnd;
        static const QString invalidGradientLine;
        static const QString invalidGradStopLine;
};

#endif
