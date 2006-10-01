 
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

#ifndef gradientdialog_h
#define gradientdialog_h


#include "resourcedialog.h"
#include "resourcekey.h"
#include "gradient.h"

#include <QHash>

class GradientWidget;
class FlagButtonGroup;


class GradientEditor : public ResourceEditor
{
        Q_OBJECT
public:
        GradientEditor( Gradient& gradient, QWidget* parent = 0 );

        virtual void updateData();
        
private slots:
        void typeChanged( int type );
        
private:
        Gradient& gradient_;
        GradientWidget* gradWidget_;
        FlagButtonGroup* gradType_;
};



#endif
