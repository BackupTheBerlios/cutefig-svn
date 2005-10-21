 
/*****************************************************************************
**
**  $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2004 Johannes Mueller, johmue@users.sourceforge.net
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

#ifndef strokedemo_h
#define strokedemo_h

#include "stroke.h"

#include <QFrame>

class StrokeDemo : public QFrame
{
        Q_OBJECT
public:
        StrokeDemo( const Stroke& stroke, QWidget* parent = 0 );
        ~StrokeDemo() {}

        void setStroke( const Stroke& stroke );

private:
        virtual void paintEvent( QPaintEvent* e );

        Stroke stroke_;
};      

#endif
