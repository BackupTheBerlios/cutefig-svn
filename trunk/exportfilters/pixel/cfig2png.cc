 
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
#include "fig.h"

#include <QString>

#include <QDebug>

class PNGFilterApp : public PixfilterApp
{
public:
        PNGFilterApp( int& argc, char* argv[] )
                : PixfilterApp( argc, argv )
        {
                format_ = "png";
                setupUsageString();
        }

private:
        int parseCommandLine();
        void setupUsageString();
};

int PNGFilterApp::parseCommandLine() 
{
        int optind;
        int pr = getOptions( optind );

        if ( pr )
                return pr;

        setupFileLists( optind );
        
        return 0;
}

void PNGFilterApp::setupUsageString() 
{
        usageString_ = tr("This is cfig2png, a part of the CuteFig project")
                       + " " + Msgs::version + "\n\n" + tr(
"Usage:\n"
"cfig2png [options] [file(s)]\n"
"\n"
"cfig2png converts CuteFig figures specified in file(s) to a png file.\n"
"If no files are specified it reads from stdin and writes to stdout.\n"
"Otherwise it reads the specified files and writes the output to a\n"
"corresponding png file.\n"
"\n"
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
"                                transparent.\n"
"        -q, --quality=val       Sets the quality to val. Must be an integer\n"
"                                value between 0 (low) and 100 (high quality).\n"
"                                Defaults to 100.\n"
"        -g, --gamma=val         Sets the gamma value to val.\n"
"        -h, --help              Prints this message and terminates successfully\n"
//-------------------------------------------------------------------------------//
"\n" 
                )
             + Msgs::bugreportsTo + "\n";
}

int main( int argc, char* argv[] )
{
        PNGFilterApp app( argc, argv );
        app.exec();
}
