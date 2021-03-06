
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

#ifndef pattern_h
#define pattern_h

#include "typedefs.h"
#include "figure.h"

#include <QList>

template<typename Resource> class TResourceIO;

class QBrush;
class QMatrix;

class Pattern;
unsigned int qHash( const Pattern& p );

class Pattern 
{
public:
        friend class TResourceIO<Pattern>;
        friend unsigned int qHash( const Pattern& p );
        
	Pattern();
	explicit Pattern( const ObjectList& obs );
	
	const ObjectList& objects() const { return figure_.objects(); }
	void setObjectList( const ObjectList& obs );
	
	const QBrush brush( const QRectF& rect, const QMatrix& m ) const;

private:
        Figure figure_;
};




#endif
