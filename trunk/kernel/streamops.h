 
/*****************************************************************************
**
**  $Id$
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

#ifndef streamops_h
#define streamops_h

#include <iostream>

class QTextStream;
class QString;
class QColor;
class QSizeF;
class ResourceKey;
class Stroke;
class Pen;
class QDate;


QString percentDecode( const QString& s );
QTextStream& operator>> ( QTextStream&, QColor& );
QTextStream& operator>> ( QTextStream&, QSizeF& );
QTextStream& operator>> ( QTextStream&, ResourceKey& );
QTextStream& operator>> ( QTextStream&, Pen& );
QTextStream& operator>> ( QTextStream&, Stroke& );
QTextStream& operator>> ( QTextStream&, QDate& );

QString percentEncode( const QString& s );
QTextStream& operator<< ( QTextStream&, const QColor& );
QTextStream& operator<< ( QTextStream&, const QSizeF& );
QTextStream& operator<< ( QTextStream&, const Stroke& );
QTextStream& operator<< ( QTextStream&, const ResourceKey& );
QTextStream& operator<< ( QTextStream&, const QDate& d );



#endif
