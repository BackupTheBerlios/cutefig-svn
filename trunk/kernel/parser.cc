 
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

/** \class Parser
 *
 *  The Parser is there to parse input files containing data of a
 *  figure. It therefore uses std::istringstream, as QTextStream does not
 *  have something like iostate. There are two streams:
 *      - QTextStream* fileStream_, which is there to read lines out of the 
 *        inputfile. 
 *      - std::istringstream stream_ to parse a single line.
 */
 
#include "parser.h"
#include "allobjects.h"
#include "figure.h"
#include "reslib.h"
#include "strokelib.h"
#include "gradient.h"

#include <QTextStream>
#include <QColor>

#include <sstream>
#include <cctype>
#include <QDebug>

/** Puts the next word of the a std::istream into a QString. 
 *
 */
std::istream &operator>> ( std::istream &is, QString &_s ) 
{
        std::string s;
        is >> s;
        _s = QString::fromUtf8( s.c_str() );
        
        return is;
}

int get_colorByte( std::istream& is )
{
         int v = toupper(is.get()) - '0';
         if ( v > 9 )
                 v -= 7;
         if ( v < 0 || v > 15 )
                 is.setstate( is.rdstate() | std::istream::failbit );
         return v;
}

int get_colorPart( std::istream& is )
{
        int v = get_colorByte( is );
        v = v << 4;
        v |= get_colorByte( is );
        return v;
}
        

/** Tries to interpret the next word of an std::istream as a
 * QColor. On failure the failbit of the std::istream is set to true.
 */
std::istream &operator>> ( std::istream &is, QColor &color )
{
        char c;
        do 
                c = is.get();
        while( isspace( c ) );
        
        if ( c != '#' ) {
                is.setstate( is.rdstate() | std::istream::failbit );
                return is;
        }
        
        color.setRed( get_colorPart( is ) );
        color.setGreen( get_colorPart( is ) );
        color.setBlue( get_colorPart( is ) );

        c = is.peek();
        if ( !is.eof() ) {
                if ( !isspace( c ) && c > 0 ) 
                        color.setAlpha( get_colorPart( is ) );
        }
        
        return is;
}

std::istream& operator>> ( std::istream &is, ResourceKey& key )
{
        char c;
        do
                c = is.get();
        while ( isspace(c) );

        if ( c == '%' ) {
                key = ResourceKey();
                return is;
        }
                
        QString keyString;
        is >> keyString;
        
        switch ( c ) {
            case '&':
                    key = ResourceKey( keyString, ResourceKey::BuiltIn );
                    break;
            case '*':
                    key = ResourceKey( keyString, ResourceKey::InFig );
                    break;
            default:
                    is.setstate( is.rdstate() | std::istream::failbit );
                    key = ResourceKey();
        }
        
        return is;
}                        

std::istream& operator>> ( std::istream &is, Stroke& s )
{
        StrokeLib& sl = StrokeLib::instance();

        char c;
        do 
                c = is.get();
        while ( isspace(c) );

        is.putback( c );
        
        if ( c == '#' ) {
                QColor color;
                if (is >> color)
                        s = Stroke( color );
        }
        else {
                ResourceKey key;
                if (is >> key) {
                        if ( !key.isValid() ) {
                                s = Stroke();
                        }
                        else if ( !sl.contains( key ) ) 
                                is.setstate( is.rdstate() | std::istream::failbit );
                        else 
                                s = sl[key];
                }
        }
                
        return is;
}

std::istream& operator>> ( std::istream &is, Pen& pen )
{
        double lw;
        ResourceKey dashKey;
        int cs, js;
        
        is >> lw >> dashKey >> cs >> js;

        if ( !is.fail() ) {
                pen.setWidth( lw );
                pen.setCapStyle( (Qt::PenCapStyle) cs );
                pen.setJoinStyle( (Qt::PenJoinStyle) js );
                pen.setDashes( dashKey );
        }

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


/** The main function of the Parser. Parses the input file, gives the
 *  DrawObjects found to the figure and returns an errorreport that
 *  can be displayed to the user.
 */
QString Parser::parse()
{
        figure_->takeDrawObjects( parseLoop() );
        return errorReport_;
}


/** The parsing loop. Parses the input file until eof or the end of a
 *  compound is found and returns the DrawObjects in an ObjecList. If
 *  the begining of a compound is found, a compound is created and
 *  filled with the ObjectList that a recursive call returned.
 */
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
                        QPointF p = parsePoint();
                        if ( !p.isNull() )
                                (*pa)[i++] = p;
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

                if ( itemType_ == "color" ){
                        pushColor();
                        continue;
                }

                if ( itemType_ == "gradient" ) {
                        pushGradient();
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
                parseError( unexpectedEnd );
        
        return olist;
}

void Parser::pushDashes()
{
        QString keyString;
        stream_ >> keyString;
        
        ResourceKey key( keyString, ResourceKey::InFig );

        DashesLib& dl = DashesLib::instance();

        if ( dl.contains( key ) ) {
                parseError( "Message Missing" );
                return;
        }
        
        Dashes dsh = parseDashes( stream_ );        
        if ( dsh.size() < 2 ) {
                parseError( invalidDashLine );
                return;
        }

        dl.insert( key, dsh );
        dashList_ << key;
}

void Parser::pushColor()
{
        QString keyString;
        
        QColor color;

        stream_ >> keyString >> color;

        ResourceKey key( keyString, ResourceKey::InFig );

        if ( stream_.fail() ) {
                parseError( invalidColor );
                return;
        }

        StrokeLib& sl = StrokeLib::instance();        
        if ( !sl.contains( key ) ) {
                Stroke s( color );
                s.setKey( key );
                sl.insert( key, s );
        }        
}

void Parser::pushGradient()
{
        QString keyString;
        std::string type;
        QColor startColor, endColor;
        double x1,x2, y1,y2;
 
        Gradient gradient;

        stream_ >> keyString >> type >> startColor >> endColor >> x1 >> y1 >> x2 >> y2;

        ResourceKey key( keyString, ResourceKey::InFig );
        
        QPointF startPoint( x1,y1 );
        QPointF endPoint( x2,y2 );
        
        if ( type == "radial" ) {
                double rad;
                stream_ >> rad;
                gradient = Gradient( Gradient::Radial, startPoint, endPoint );
                gradient.setRadius( rad );
        }
        else 
                gradient = Gradient( Gradient::Linear, startPoint, endPoint );

        if ( stream_.fail() ) {
                parseError( invalidGradientLine );
                return;
        }       

        gradient.setColorAt( 0.0, startColor );

        QColor cl;
        double pos;
        
        readLine();
        
        while ( itemType_ != "gradend" ) {
                stream_ >> pos >> cl;
                if ( stream_.fail() )
                        parseError( invalidGradStopLine );
                else
                        gradient.setColorAt( pos, cl );
                
                readLine();
        }

        gradient.setColorAt( 1.0, endColor );

        StrokeLib& sl = StrokeLib::instance();
        
        if ( !sl.contains( key ) ) {
                Stroke stroke;
                stroke.setGradient( gradient );
                stroke.setKey( key );
                sl.insert( key, stroke );
        }
}

/** Takes the next line of the input file and puts it into the
 *  sstream_. Comments are removed and put into objectComment_. The
 *  first word of the input line is put into itemType_ If the
 *  fileStream_ is finished false is returned, otherwise true;
 */
bool Parser::readLine()
{
        QString s;     
        s = fileStream_->readLine();
        line_++;

        while ( s[0] == '#' || s.isEmpty() ) {
                if ( !s.isEmpty() ) {
                        s.remove( 0, 1 );
                        objectComment_ += s + '\n';
                }
                s = fileStream_->readLine();
                if ( fileStream_->atEnd() )
                        return false;
                line_++;
        }       
        
        stream_.clear();
        stream_.str( std::string( s.trimmed().toUtf8() ) );
        stream_ >> itemType_;
        
        return true;
}

/** Tries to interpret the next two words of sstream_ as to
 *  doubles. If that succeeds these values are put intp pa[i].
 */
QPointF Parser::parsePoint()
{
        double x,y;
        QPointF p;
        
        if ( (stream_ >> x >> y) )
                p = QPointF( x,y );
        else
                parseError( invalidPoint );

        return p;
}

/** Parses the generic line of itemType_ == "object". It creates the
 *  DrawObject and sets its genereic data. Finally it returns a poiter
 *  to that DrawObject. If an error occurs the DrawObject is deleted
 *  if necessary and a null pointer is returned.
 */
DrawObject * Parser::parseGenericData( uint &npoints, QPolygonF*& pa )
{
        QString obType;
        Stroke stroke, fill;
        ResourceKey dashKey;
        int depth;
        Pen pen;

        stream_ >> obType >> npoints;
        if ( stream_.fail() ) {
                parseError( invalidStandardLine, Discarding );
                npoints = 0;
                return 0;
        }

        if ( !( stream_ >> pen ) ) 
                parseError( tr("Invalid pen.") );

        if ( !( stream_ >> stroke >> fill ) )
                parseError( tr("Invalid strokes.") );

        if ( !( stream_ >> depth ) ) {
                parseError( tr("Invalid depth, assuming 50") );
                depth = 50;
        }
        
        DrawObject *o = createObject( obType );
        if ( !o ) {
                npoints = 0;
                parseError( invalidObjectData, Discarding );
                return 0;
        }

        if ( npoints < o->minPoints() ) {
                parseError( notEnoughPoints.arg(obType).arg(npoints).arg(o->minPoints()),
                            Discarding );
                delete o;
                npoints = 0;
                return 0;
        }
        
        o->setPen( pen );
        o->setStroke( stroke );
        o->setFill( fill );
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
        t.append("\n    *** ");
        t.append( s );        
        errorReport_.append( t );
        
        if ( sev == Discarding ) {
                errorReport_.append(" ");
                errorReport_.append( tr("Object discarded.") );
        }
        errorReport_.append("\n");
}

Dashes Parser::parseDashLine( const std::string& s )
{
        std::istringstream is( s );
        return parseDashes( is );
}

Dashes parseDashes( std::istringstream& is )
{
        Dashes d( 0 );
        double val;

        while ( !is.eof() )
                if ( is >> val && val > 0 )
                        d.push_back( val );
        
        return d;
}

const QString Parser::unknownItemType = tr("Ignoring unknown item %1");
const QString Parser::unknownObject = tr("Ignoring unknown object %1");
const QString Parser::invalidStandardLine = tr("Parsing of standard data failed");
const QString Parser::invalidObjectData = tr("Parsing of object specific data failed");
const QString Parser::notEnoughPoints = tr("Not enough points for %1. Found %2, need %3.");
const QString Parser::wrongPointNumber = tr("Found %1 points while expecting %2.");
const QString Parser::ignoringPoint = tr("Ignoring superflous point"); // not used
const QString Parser::invalidPenStyle = tr("Invalid pen style: assuming solid line.");
const QString Parser::invalidJoinStyle = tr("Invalid join style: assuming miter.");
const QString Parser::invalidCapStyle = tr("Invalid cap style: assuming flat.");
const QString Parser::invalidPoint = tr("Ignoring invalid point");
const QString Parser::invalidColor =  tr("Invalid colour: assuming black.");
const QString Parser::invalidDashLine = tr("Parsing of dashline failed");
const QString Parser::undefinedDashes = tr("Dashtype %1 undefined. Assuming solid line");
const QString Parser::compound_end_without_compound = tr("Received compound end without compund.");
const QString Parser::unexpectedEnd = tr("Unexpected end of file.");
const QString Parser::invalidGradientLine = tr("Invalid gradient line.");
const QString Parser::invalidGradStopLine = tr("Invalid gradient stop line.");



