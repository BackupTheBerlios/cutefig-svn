 
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

#ifndef percentvalidatior_h
#define percentvalidatior_h

#include <QIntValidator>

#include <limits>

class PercentValidator : public QIntValidator
{
        Q_OBJECT
public:
        PercentValidator( QObject* parent )
                : QIntValidator( 1, std::numeric_limits<int>::max(), parent ) {}
        ~PercentValidator() {}

        void setBottom( int bottom );
        void setRange( int min, int max );
        
        virtual QValidator::State validate( QString& input, int& pos ) const;
        virtual void fixup( QString& intput );
};

#endif
