 
/*****************************************************************************
**
**  @version $Id: colorbutton.h 16 2005-06-26 22:00:14Z joh $
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

#ifndef gradientbutton_h
#define gradientbutton_h

#include "gradient.h"

#include <QPushButton>

class GradientButton : public QPushButton
{
        Q_OBJECT
public:
        GradientButton( const Gradient& gradient, QWidget* parent = 0 );
        ~GradientButton() {}

        void setGradient( const Gradient& gradient );
        Gradient getGradient() const { return gradient_; }
        
signals:
        void gradientChanged( const Gradient& );

public slots:
        void changeGradient();

private:
        Gradient gradient_;
};

#endif
