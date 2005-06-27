 
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

 
#include "parser.h"
#include "allobjects.h"
#include "figure.h"
#include "reslib.h"

#include <QTextStream>
#include <QColor>

#include <sstream>

#include <QDebug>

std::istream &operator>> ( std::istream &is, QString &_s ) 
{
        std::string s;
        is >> s;
        _s = QString::fromUtf8( s.c_str() );
        
        return is;
}

std::istream &operator>> ( std::istream &is, QColor &c )
{
        QString s;
        double alpha = -1;
        is >> s >> alpha;
        c = QColor( s );
        c.setAlphaF( alpha );
        if ( !c.isValid() )
                is.setstate( is.rdstate() | std::istream::failbit );

        return is;
}

Dashes parseDashes( std::istringstream& is );

Parser::Parser( QTextStream *ts, Figure *f, 
                QObject * parent )
        : QObject( parent ),
          fileStream_( ts ),
          line_( 0 ),
          figure_( f ),
          errorReport_( QString() )
{
        registeredTypes_.insert( "ellipse", &Parser::createEllipse );
        registeredTypes_.insert( "polyline", &Parser::createPolyline );
        registeredTypes_.insert( "polygon", &Parser::createPolygon );
}

QString Parser::parse()
{
        figure_->takeDrawObjects( parseLoop() );
        return errorReport_;
}

ObjectList Parser::parseLoop( bool parsingCompound )
{
        uint npoints = 0;
        uint i = 0;
        
        QPolygonF* pa;
        DrawObject* o = 0;
        ObjectList olist;

        while ( readLine() ) {

                if ( itemType_ == "point" ) {
                        if ( !o ) {
                                parseError( ignoringPoint );
                                continue;
                        }
                        parsePoint( *pa, i );
                        if ( npoints == i ) {
                                o->setComment( objectComment_ );
                                objectComment_ = QString();
                                olist.push_back( o );
                                o = 0;
                        }
                        continue;
                }
                else if ( o ) {
                        parseError(wrongPointNumber.arg(i).arg(npoints), Discarding);
                        delete o;
                }

                if ( itemType_ == "object" ) {
                        o = parseGenericData( npoints, pa );
                        i = 0;
                        continue;
                }

                if ( itemType_ == "dashes" ) {
                        pushDashes();
                        continue;
                }

                if ( itemType_ == "compound_begin" ) {
                        olist.push_back( new Compound( parseLoop( true ), figure_ ) );
                        continue;
                }

                if ( itemType_ == "compound_end" ) {
                        if ( !parsingCompound )
                                parseError( compound_end_without_compound );
                        break;
                }

                parseError( unknownItemType.arg( itemType_ ) );

        }

        if ( itemType_ != "compound_end" && parsingCompound )
                parseError( fileEndBefireCompoundFinished );
        
        return olist;
}

void Parser::pushDashes()
{
        Dashes dsh = parseDashes( stream_ );        

        DashesLib& dl = DashesLib::instance();
        int key = dl.indexOf( dsh );
        if ( key == -1 ) {
                key = dl.size();
                dl << dsh;
        }
        dashList_ << key;
}

bool Parser::readLine()
{
//         if ( !stream_.eof() )
//                 return true;

        QString s;     
        s = fileStream_->readLine();
        
        line_++;

        while ( s[0] == '#' ) {
                s.remove( 0, 1 );
                objectComment_ += s + '\n';     
                s = fileStream_->readLine();
                line_++;
        } 
       
        if ( s.isNull() )
                return false;
        
        stream_.clear();
        stream_.str( std::string( s.trimmed().toUtf8() ) );
        stream_ >> itemType_;
        
        return true;
}

void Parser::parsePoint( QPolygonF& pa, uint &i )
{
        double x,y;
        
        if ( (stream_ >> x >> y) )
                pa[i++] = QPointF( x,y );
        else
                parseError( invalidPoint );
}

// QBrush Parser::parsePattern( int lines )
// {
//         char** xpm;
//         int i;
        
//         for ( i=0; i<lines; i++ ) {
//                 if ( fileStream_->atEnd() )
//                         break;
//                 *fileStream_ >> xpm[i];
//         }

//         if ( i>lines )
//                 return QBrush( Qt::NoBrush );
        
//         return QBrush( Qt::white, QPixmap( (const char**)xpm ) );
// }

DrawObject * Parser::parseGenericData( uint &npoints, QPolygonF*& pa )
{
        QString obType;
        QColor pc, fc;
        int dsh, cs, js, pattern, depth;
        double lw;

        stream_ >> obType >> npoints >> lw >> dsh >> cs >> js >> pc >> fc >> pattern >> depth;

        if ( stream_.fail() ) {
                parseError( invalidStandardLine, Discarding );
                npoints = 0;
                return 0;
        }
                        
        DrawObject *o = createObject( obType );
        if ( !o ) {
                npoints = 0;
                parseError( invalidObjectData, Discarding );
                return 0;
        }

        Pen pen;
//        qDebug() << dsh << dashList_.size();
        if ( (dsh > dashList_.size()-1) || (dsh < -6) ) {
                parseError( undefinedDashes.arg( dsh ) );
                dsh = -5;
        }
        
        if ( dsh >= 0 )
                dsh = dashList_[dsh];
        
        pen.setDashes( dsh );
//        qDebug() << dsh;

        pen.setColor( pc );
        pen.setWidth( lw );

        pen.setCapStyle( (Qt::PenCapStyle) cs );
        pen.setJoinStyle( (Qt::PenJoinStyle) js );
        
        o->setPen( pen );
        o->setDepth( depth );
        
        pa = &o->points();
        pa->resize( npoints );
        
        return o;
}               

DrawObject * Parser::createObject( QString type )
{
        creatorType c = registeredTypes_[type];
        if ( c )
                return (this->*c)();

        parseError( unknownObject.arg( type ) );
        return 0;
}

DrawObject * Parser::createEllipse()
{
        int circle, byRadius;
        float angle;

        stream_ >> circle >> byRadius >> angle;
        
        if ( stream_.fail() )
                return 0;
        
        Ellipse *e = new Ellipse( figure_ );
        e->setIsCircle( circle );
        e->setSpecByRadii( byRadius );
        e->setAngleNew( angle );
        
        return e;
}

DrawObject * Parser::createPolyline()
{
        return new Polyline( figure_ );
}

DrawObject * Parser::createPolygon()
{
        return new Polygon( figure_ );
}


void Parser::parseError( QString s, ErrorSeverity sev )
{
        QString t;
        if ( sev == Fatal ) 
                t = tr("Fatal error");
        else 
                t = tr("Parser error");
        t.append( tr(" in line %1: ").arg( line_ ) );
        t.append( s );        
//        qDebug( t );
        t.append("\n");
        errorReport_.append( t );
        
        if ( sev == Discarding )
                errorReport_.append( tr("Object discarded.") );
}

Dashes Parser::parseDashLine( const std::string& s )
{
        std::istringstream is( s );
        return parseDashes( is );
}

Dashes parseDashes( std::istringstream& is )
{
        Dashes d( 0 );
 
        while ( !is.eof() ) {
                double val;
                if (is >> val)
                        d.push_back( val );
        }
        
        return d;
}

const QString Parser::unknownItemType = tr("Ignoring unknown item %1");
const QString Parser::unknownObject = tr("Ignoring unknown object %1");
const QString Parser::invalidStandardLine = tr("Parsing of standard data failed");
const QString Parser::invalidObjectData = tr("Parsing of object specific data failed");
const QString Parser::wrongPointNumber = tr("Found %1 points while expecting %2.");
const QString Parser::ignoringPoint = tr("Ignoring superflous point"); // not used
const QString Parser::invalidPenStyle = tr("Invalid pen style: assuming solid line.");
const QString Parser::invalidJoinStyle = tr("Invalid join style: assuming miter.");
const QString Parser::invalidCapStyle = tr("Invalid cap style: assuming flat.");
const QString Parser::invalidPoint = tr("Ignoring invalid point");
const QString Parser::invalidColor =  tr("Invalid colour: assuming black.");
const QString Parser::undefinedDashes = tr("Dashtype %1 undefined. Assuming solid line");
const QString Parser::compound_end_without_compound = tr("Received compound end without compund.");
const QString Parser::fileEndBefireCompoundFinished = tr("File ended before compound finished.");
