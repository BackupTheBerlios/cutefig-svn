 
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
#include "figure.h"
#include "reslib.h"
//#include "strokelib.h"
// #include "gradient.h"
#include "resourceio.h"
#include "objecthandler.h"
#include "drawobject.h"
#include "compound.h"
#include "streamops.h"

#include <QTextStream>
#include <QColor>
#include <QCoreApplication>

#include <sstream>
#include <cctype>
#include <QDebug>

        

Parser::Parser( QTextStream& ts  )
        : fileStream_( ts ),
          line_( 0 ),
          errorReport_( QString() )
{
}


/** The main function of the Parser. Parses the input file, gives the
 *  DrawObjects found to the figure and returns an errorreport that
 *  can be displayed to the user.
 */
QString Parser::parse( QTextStream& ts, Figure* f )
{
        Parser p( ts );
        f->takeDrawObjects( p.parseLoop() );
        return p.errorReport_;
}

QString Parser::parseResLibs( QTextStream& ts )
{
        Parser p( ts );
        p.resourceParseLoop();
        return p.errorReport_;
}


/** The parsing loop. Parses the input file until eof or the end of a
 *  compound is found and returns the DrawObjects in an ObjecList. If
 *  the begining of a compound is found, a compound is created and
 *  filled with the ObjectList that a recursive call returned.
 */
ObjectList Parser::parseLoop( bool parsingCompound )
{
        int npoints = 0;
        int i = 0;
        
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
                
                if ( itemType_ == "compound_begin" ) {
                        olist.push_back( new Compound( parseLoop( true ) ) );
                        continue;
                }

                if ( itemType_ == "compound_end" ) {
                        if ( !parsingCompound )
                                parseError( compound_end_without_compound );
                        break;
                }

                if ( itemType_ == "resource" ) {
                        parseResource( ResourceKey::InFig );
                        continue;
                }
                
                parseError( unknownItemType.arg( itemType_ ) );

        }

        if ( itemType_ != "compound_end" && parsingCompound )
                parseError( unexpectedEnd );
        
        return olist;
}

void Parser::resourceParseLoop()
{
        while ( readLine() )
                if ( itemType_ == "resource" )
                        parseResource( ResourceKey::InLib );
}

void Parser::parseResource( ResourceKey::Flags flags )
{
        QString keyWord;
        stream_ >> keyWord;

        ResourceIO* resIO = ResourceIOFactory::getResourceIO( keyWord );
        
        if ( !resIO ) {
                parseError( unknownResourceType.arg( keyWord ) );
                return;
        }

        QString rks;
        stream_ >> rks;

        bool rKeyFound;
        int hashsum = resIO->hashSum( ResourceKey::inLib( rks ), &rKeyFound );
        
        
        bool parseit = true;
        
        int savedHashsum;
        stream_ >> savedHashsum;

        if ( hashsum ) {

                if ( hashsum == savedHashsum ) {
                        QString s;
                        do {
                                fileStream_ >> s;
                                fileStream_.readLine();
                        } while ( s != "resource_end" );
                        
                        parseit = false;
                }
        }

        if ( parseit ) {
                if ( resIO->parseResource( QString(), stream_ ) ) {
                        readLine();
                        while ( itemType_ != "resource_end" && !resIO->failed() ) {
                                resIO->parseResource( itemType_, stream_ );
                                readLine();
                        } 
                }

                resIO->postProcessResource();
                
                if ( resIO->failed() )
                        parseError( resIO->errorString() );
                else
                        resIO->pushResource( ResourceKey( rks, flags ) );
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
        s = fileStream_.readLine();
        line_++;

        while ( s[0] == '#' || s.isEmpty() ) {
                if ( !s.isEmpty() ) {
                        s.remove( 0, 1 );
                        objectComment_ += s + '\n';
                }
                s = fileStream_.readLine();
                if ( fileStream_.atEnd() )
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
                    key = ResourceKey::builtIn( keyString );
                    break;
            case '*':
                    key = ResourceKey::inFig( keyString );
                    break;
            default:
                    is.setstate( is.rdstate() | std::istream::failbit );
                    key = ResourceKey();
        }
        
        return is;
}

void Parser::parseStroke( Stroke& s )
{
        char c;
        do 
                c = stream_.get();
        while ( isspace(c) );

        stream_.putback( c );
        
        if ( c == '#' ) {
                QColor color;
                if (stream_ >> color)
                        s = Stroke( color );
        } else {
                ResourceKey key;
                
                s = Stroke();
                if ( (stream_ >> key) && key.isValid() ) {
                
                        QString kw;
                        stream_ >> kw;
                        
                        if ( kw == "color" ) 
                                s.setColor( key );
                        else if ( kw == "gradient" )
                                s.setGradient( key );
                }
        }
}

std::istream& operator>>( std::istream& is, Pen& pen )
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

 

/** Parses the generic line of itemType_ == "object". It creates the
 *  DrawObject and sets its genereic data. Finally it returns a poiter
 *  to that DrawObject. If an error occurs the DrawObject is deleted
 *  if necessary and a null pointer is returned.
 */
DrawObject * Parser::parseGenericData( int &npoints, QPolygonF*& pa )
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

        parseStroke( stroke );
        parseStroke( fill );
        
        if ( !( stream_ >> depth ) ) {
                parseError( tr("Invalid depth, assuming 50") );
                depth = 50;
        }

        DrawObject* o = ObjectHandler::getDrawObject( obType, stream_ );
        if ( !o ) {
                npoints = 0;
                parseError( invalidObjectData, Discarding );
                return 0;
        }

        if ( o->canHaveArrows() ) {
                ResourceKey s, e;
                stream_ >> s;
                if ( s.isValid() ) {
                        double w, l;
                        stream_ >> w >> l;
                        o->setStartArrow( Arrow( s, w, l ) );
                }

                stream_ >> s;
                if ( s.isValid() ) {
                        double w, l;
                        stream_ >> w >> l;
                        o->setEndArrow( Arrow( s, w, l ) );
                }
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

QString Parser::tr( const char* source )
{
        return QCoreApplication::translate("Parser", source );
}

const QString Parser::unknownItemType = tr("Ignoring unknown item %1");
const QString Parser::unknownResourceType = tr("Unknown resource type %1");
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



