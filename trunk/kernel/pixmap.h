 
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

#include <QImage>
#include <QString>

class Pixmap;
unsigned int qHash( const Pixmap& pm );

//! Wraps QPixmap to be used in a Stroke
class Pixmap 
{
public:
        friend unsigned int qHash( const Pixmap& pm );
        
	Pixmap();
//        Pixmap( const Pixmap& other );
//        ~Pixmap();        

        //       Pixmap& operator=( const Pixmap& other );
        
        void setImage( const QImage& img );
	const QString loadFromFile( const QString& filename );

	const QImage image() const { return image_; }
	const QString fileName() const { return fileName_; }

        QByteArray format() const { return format_; }
        void setFormat( const QByteArray& f ) { format_ = f; }
        
	bool isFromFile() const;

private:
        QImage image_;
        QByteArray format_;
	QString fileName_;
};


#endif
