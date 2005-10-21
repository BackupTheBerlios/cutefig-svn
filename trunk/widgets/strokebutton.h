 
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

#ifndef strokebutton_h
#define strokebutton_h

#include "typedefs.h"
#include "stroke.h"
#include <QPushButton>


class StrokeButton : public QPushButton 
{
        Q_OBJECT
public:
        StrokeButton( const Stroke& initial, const ResourceKeyList& keys, QWidget* parent = 0 );
        ~StrokeButton() {}

        void setStroke( const Stroke& stroke );
        Stroke getStroke() const { return stroke_; }

signals:
        void strokeChanged( const ResourceKey& key );

private slots:
        void changeStroke();

private:
        Stroke stroke_;
        const ResourceKeyList& keys_;
};

#endif
