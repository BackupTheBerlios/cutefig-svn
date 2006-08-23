 
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

#include "pixoutput.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QByteArray>

class Figure;

class PixfilterApp : public QApplication
{
public:
        PixfilterApp( int& argc, char* argv[] );

        int exec();

protected:
        QByteArray format_;
        bool toStdout_;
        QString usageString_;
        QStringList infileList_, outfileList_;
        QTextStream cerr;
        Figure* figure_;
        PIXOutput pixout_;

        bool verbose_;

        QFile infile_, outfile_;

        virtual int parseCommandLine();
        int getOptions( int& _optind );
        void setupFileLists( int optind );
        int parseResult_;
        int exportFigure();

        void verbose( const QString& s );
        void usage();
        void printOptions();
        void printImageFormats();

private:
        int openInfile( const QString& filename );
        int openOutfile( const QString& filename );
        void setupUsageString();
};
