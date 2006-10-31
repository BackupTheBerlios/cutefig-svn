 
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
**  but WITHOUT ANY WARRANTY; with;out even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
******************************************************************************/

#ifndef pixmap_h
#define pixmap_h

#include <QPixmap>
#include <QString>

class Pixmap;
int qHash( const Pixmap& pm );

        
class Pixmap 
{
public:
        friend int qHash( const Pixmap& pm );
        
	Pixmap();
        Pixmap( const Pixmap& other );
        ~Pixmap();        

        Pixmap& operator=( const Pixmap& other );
        
        void setPixmap( const QPixmap& pm );
	bool loadFromFile( const QString& filename );

	const QPixmap qpixmap() const;
	const QString fileName() const { return fileName_; }

	bool isFromFile() const;

private:
        QPixmap& pixmap();
        
	QString fileName_;
	QPixmap* pixmap_;
};


int qHash( const Pixmap& pm );


#endif
