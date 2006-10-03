 
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

#ifndef pixoutput_h
#define pixoutput_h

#include "exportfilter.h"

#include <QColor>
#include <QString>

class PixoutDialog;

class PIXOutput : public ExportFilter
{
public:
        PIXOutput( QByteArray format );
        PIXOutput( const Figure* figure, QFile* file, QByteArray format = QByteArray() );

        friend class PixoutDialog;
        
        void setFormat( const QByteArray& format ) { format_ = format; }
        void setScale( double s );
        void setXres( int x );
        void setYres( int y );
        void setBackground( const QColor& c ) { backgroundColor_ = c; }
        void setQuality( int q ) { quality_ = q; }
        void setGamma( double g ) { gamma_ = g; }
	void setWithPaper( bool w ) { withPaper_ = w; }

        void exportFigure();

        bool error() const { return error_; }
        QString errorString() const { return errorString_; }

private:
        QByteArray format_;
        int xres_, yres_;
        double scale_;
        QColor backgroundColor_;
        int quality_;
        double gamma_;
	bool withPaper_;
	
        bool error_;
        QString errorString_;
};
#endif
