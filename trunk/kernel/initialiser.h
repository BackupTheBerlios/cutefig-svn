 
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

#ifndef initialiser_h
#define initialiser_h

#include <QHash>
#include <QString>

namespace Initialiser
{
        template<typename T> class AutoHash;
};

template<typename T> class Initialiser::AutoHash
{
public:
        AutoHash()
                : hash_() 
        {
                init();
        }
        
        T*& operator[]( const QString& keyword )
        {
                return hash_[keyword];
        }

private:
        AutoHash( const AutoHash& );
        
        QHash<QString, T*> hash_;

        void init();
};


#endif
