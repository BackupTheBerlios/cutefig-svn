 
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

#ifndef exportfilter_h
#define exportfilter_h

#include <QString>
#include <QSize>

class Figure;
class QFile;

class ExportFilter
{
public:
        ExportFilter( const Figure* f = 0, QFile* file = 0 )
                : figure_ ( f ), file_( file ) {}

        virtual ~ExportFilter() {}

        virtual void exportFigure() = 0;
        virtual const QString filterName() const = 0;

// MIME-stuff missing
protected:
        const Figure* figure_;
        QFile* file_;

};

#endif
