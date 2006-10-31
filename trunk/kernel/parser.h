 
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

/*! \class Parser
 *  \brief Parses a input file.
 */
#include <QObject>
#include <QString>
#include <QPolygonF>
#include <QPen>
#include <QHash>

#include "typedefs.h"
#include "pen.h"
#include "stroke.h"

class QTextStream;
class DrawObject;
class Figure;




class Parser 
{
public:
        static QString parse( QTextStream& ts, Figure* f );
        static QString parseResLibs( QTextStream& ts );
        static QString parseObjects( QTextStream& ts, Figure* f );

        static Dashes parseDashLine( const std::string& s );
        
private:
        enum ErrorSeverity { Warning=0x0000, Discarding=0x0001, Fatal=0x0002 };
        
        Parser( QTextStream& ts, Figure* f = 0 );

        
        QString parseHeader();
        QString parseMetaData();
        
        ObjectList parseLoop( bool parsingCompound = false );
        void resourceParseLoop();
        QPolygonF getPoints( uint n );
        bool readLine();


        QTextStream& fileStream_;

        Figure* figure_;

        double figureUnit_;

        QString itemType_;
        QString currentLine_;
        
        QTextStream stream_;
        unsigned int line_;

        QString errorReport_;

        QString objectComment_;

        DrawObject* parseGenericData();

        QPointF parsePoint();
        void parseResource( ResourceKey::Flags flag );

        static QString tr( const char* source );
        
        QPen setUpPen( int w, int s, int cs, int js, QColor c );

        QString makeErrorLine( const QString& s, ErrorSeverity sev = Warning );
        void parseError( const QString& s, ErrorSeverity sev = Warning );

        static const QString unknownItemType;
        static const QString unknownResourceType;
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
