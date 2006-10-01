 
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

#include "percentvalidator.h"

#include <limits>

#include <QDebug>


PercentValidator::PercentValidator( QObject* parent )
        : QIntValidator( 1, std::numeric_limits<int>::max(), parent ),
          lastValidated_( 100 )
{
}

void PercentValidator::setRange( int _bottom, int _top )
{
        QIntValidator::setRange( _bottom >= 1 ? _bottom : bottom(), _top );
}

QValidator::State PercentValidator::validate( QString& input, int& pos ) const
{
        input = input.trimmed();

        bool chopped = false;
        if ( input.endsWith('%') ) {
                input.chop( 1 );
                input = input.trimmed();
                chopped = true;
        }
        
        QValidator::State s = QIntValidator::validate( input, pos );

        if ( s == QValidator::Acceptable )
                lastValidated_ = input.toInt();

        if ( chopped || s == QValidator::Acceptable )
                input.append(" %");

        return s;
}


double PercentValidator::lastValidated() const
{
        return (double) lastValidated_ / 100;
}
