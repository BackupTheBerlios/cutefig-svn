 
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

#include "pixfilterapp.h"
#include "figure.h"
#include "parser.h"
#include "fig.h"

#include <getopt.h>
#include <stdlib.h>

#include <QImageWriter>

#include <QDebug>

PixfilterApp::PixfilterApp( int& argc, char* argv[] )
        : QCoreApplication( argc, argv ),
          format_(),
          toStdout_( false ),
          infileList_(),
          outfileList_(),
          cerr( stderr ),
          figure_( new Figure( this ) ),
          pixout_( figure_, &outfile_ )
{
        setupUsageString();
}

int PixfilterApp::getOptions( int& _optind )
{
        int opt;

        _optind = 0;

        for(;;) {
                int option_index;
		static struct option long_options[] = {
			{"stdout",     0, 0, 'c'},
			{"scale",      1, 0, 's'},
			{"xres",       1, 0, 'x'},
			{"yres",       1, 0, 'y'},
                        {"background", 1, 0, 'b'},
                        {"quality",    1, 0, 'q'},
                        {"gamma",      1, 0, 'g'},
			{"help",       0, 0, 'h'},
                        {"list",    0, 0, 'l'},
			{0, 0, 0, 0}
		};

		opt = getopt_long ( argc(), argv(), "cs:x:y:q:g:b:hl",
                                    long_options, &option_index);

                if ( opt == -1 )
                        break;

                bool ok;

                switch( opt ) {
                    case 'c': 
                            toStdout_ = true;
                            break;
                    case 's': {
                            double scale = QString( optarg ).toDouble( &ok );
                            if ( ok )
                                    pixout_.setScale( scale );
                    } break;
                    case 'x': {
                            int x = QString( optarg ).toInt( &ok );
                            if ( ok )
                                    pixout_.setXres( x );
                    } break;
                    case 'y': {
                            int y = QString( optarg ).toInt( &ok );
                            if ( ok )
                                    pixout_.setYres( y );
                    } break;
                    case 'b': {
                            QColor c( optarg );
                            if ( c.isValid() )
                                    pixout_.setBackground( c );
                    } break;
                    case 'q': {
                            int q = QString( optarg ).toInt( &ok );
                            if ( ok )
                                    pixout_.setQuality( q );
                    } break;
                    case 'g': {
                            double g = QString( optarg ).toDouble( &ok );
                            if ( ok )
                                    pixout_.setGamma( g );
                    } break;
                    case 'h':
                            usage();
                            return 1;
                            break;
                    case 'F':
                            printImageFormats();
                            return 1;
                    case '?':
                            return 2;
                            break;
                    default: break;
                }
        }
        _optind = optind;

        return 0;
}

int PixfilterApp::parseCommandLine()
{
        int optind; 
        int pr = getOptions( optind );

        if ( pr ) 
                return pr;

        if ( !( optind <= argc() ) ) {
                usage();
                return 2;
        }
        
        format_ = argv()[optind++];
        if ( format_.isEmpty() ) {
                cerr << "cfig2pixel: " 
                     << tr("No image format specified, use -h for help\n");
                printImageFormats();
                return 3;
        } else if ( !QImageWriter::supportedImageFormats().contains( format_ ) ) {
                cerr << "cfig2pixel: " 
                     << tr("Image format %1 not supported. Exiting.\n")
                        .arg( QString(format_) );
                printImageFormats();
                return 4;
        }

        setupFileLists( optind );

        return 0;
}

void PixfilterApp::setupFileLists( int optind )
{
        while ( optind < argc() ) {
                QString fn( argv()[optind++] );
                infileList_ << fn;
                fn.replace( QRegExp("cf?ig$", Qt::CaseInsensitive ), format_ );
                outfileList_ << fn;
        }

        if ( toStdout_ )
                if ( outfileList_.size() > 1 )
                        cerr << tr("Output do stdout only makes sense"
                                   "when prossessing a single file.\n"
                                   "Option -c --stdout ignored.\n");
                else 
                        outfileList_.clear();

    
}

int PixfilterApp::openInfile( const QString& filename )
{
        infile_.setFileName( filename );
        if ( !infile_.open( QIODevice::ReadOnly ) ) {
                cerr << tr("Failed to open %1 for reading: %2.\n")
                        .arg(filename).arg(strerror(errno));
                return errno;
        }

        return 0;
}

int PixfilterApp::openOutfile( const QString& filename )
{
        outfile_.setFileName( filename );
        if ( !outfile_.open( QIODevice::WriteOnly ) ) {
                cerr << tr("Failed to open %1 for output: %2.\n")
                        .arg(filename).arg(strerror(errno));
                return errno;
        }
        
        return 0;
}


int PixfilterApp::exec()
{
        parseResult_ = parseCommandLine();

        if ( parseResult_ )
                return parseResult_-1;

        pixout_.setFormat( format_ );

        if ( infileList_.isEmpty() ) {
                infile_.open( stdin, QIODevice::ReadOnly );
                outfile_.open( stdout, QIODevice::WriteOnly );
                return exportFigure();
        }
        
        int error;

        if ( outfileList_.isEmpty() ) {
                error = openInfile( infileList_.first() );
                if ( error )
                        return error;

                outfile_.open( stdout, QIODevice::WriteOnly );
                return exportFigure();
        }

        int retVal = 0;
        for ( int i=0; i<infileList_.size(); i++ ) {

                error = openInfile( infileList_[i] );
                if ( error )
                        return error;
                
                error = openOutfile( outfileList_[i] );
                if ( error )
                        return error;

                error = exportFigure();
                if ( error )
                        retVal = error;
        }

        return retVal;
}

int PixfilterApp::exportFigure()
{
        QTextStream ts( &infile_ );
        figure_->clear();
        Parser p( &ts, figure_ );
        QString errors = p.parse();
        infile_.close();

        if ( !errors.isEmpty() ) {
                cerr << errors << "\n";
                return 1;
        }

        pixout_.exportFigure();
        outfile_.close();
        if ( pixout_.error() ) {
                cerr << pixout_.errorString() << "\n";
                return 2;
        }

        return 0;
}

void PixfilterApp::verbose( const QString& s )
{
        if ( verbose_ )
                cerr << s;
}

void PixfilterApp::usage()
{    
        cerr << usageString_;
}

void PixfilterApp::printImageFormats()
{
        cerr << tr("Supported image formats:") << "\n";
        foreach( QByteArray f, QImageWriter::supportedImageFormats() )
                cerr << f << " ";
        cerr << "\n";
}

void PixfilterApp::setupUsageString()
{
//---------------------------------------------------------------------------------//
        usageString_ += tr("This is cfig2pixel, a part of the CuteFig project")
                        + " " + Msgs::version + "\n\n" + tr(
"Usage:\n"
"cfig2pixel format [options] [file(s)]\n"
"\n"
"cfig2pixel converts CuteFig figures specified in file(s) to a pixmap file of the\n"
"specified format If no files are specified it reads from stdin and writes to\n"
"stdout. Otherwise it reads the specified files and writes the output to a\n"
"corresponding pixmap file.\n"
"\n" );
        printOptions();
}

void PixfilterApp::printOptions()
{
        usageString_ += tr(
"Options:\n"
"        -c, --stdout            Output is written to stdout instead of a\n"
"                                regular file. This option is ignored if several\n"
"                                files are processed at once.\n"
"        -s, --scale=sale        Scales the figure by factor s, where s must be\n"
"                                a valid floating point number.\n"
"        -x, --xres=xres         Sets the horizontal resolution of the resulting\n"
"                                png to x, where x must be a valid integer\n"
"                                number. The aspect ratio of the figure is kept.\n"
"                                Will override --scale.\n"
"        -y, --yres=yres         As --xres but sets the vertical resolution.\n"
"                                If both --xres and --yres are set, the aspect\n"
"                                ratio of the figure probably will be lost.\n"
"        -b, --background=color  Sets the background color. The argument must be\n"
"                                convertable into a color. Defaults to\n"
"                                transparent. Not supported by all formats.\n"
"        -q, --quality=val       Sets the quality to val. Must be an integer\n"
"                                value between 0 (low) and 100 (high quality).\n"
"                                Defaults to 100.\n"
"        -g, --gamma=val         Sets the gamma value to val.\n"
"        -h, --help              Prints this message and terminates successfully\n"
"        -l, --list              Prints out all supported formats and returns\n"
"                                successfully.\n"
//-------------------------------------------------------------------------------//
"\n" 
                )
             + Msgs::bugreportsTo + "\n";
}
