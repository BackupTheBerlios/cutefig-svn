 
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

#include "gradientbutton.h"
#include "strokeiconengines.h"
#include "gradientdialog.h"
#include "gradient.h"
#include "stroke.h"
#include "reslib.h"

#include <QDebug>

GradientButton::GradientButton( const Gradient* gradient, QWidget* parent )
        : QPushButton( parent ),
          gradient_( 0 )
{
        setGradient( gradient );
        connect( this, SIGNAL( clicked() ), this, SLOT( changeGradient() ) );
}

void GradientButton::setGradient( const Gradient* gradient )
{
        delete gradient_;
        if ( gradient )
                gradient_ = gradient->copy();
        else {
                StrokeLib& sl = StrokeLib::instance();
                gradient_ = sl[ResourceKey("defaultGradient", ResourceKey::BuiltIn)].gradient();
        }
        
        setIcon( QIcon( new GradientIconEngine( gradient_ ) ) );
}

void GradientButton::changeGradient()
{
        GradientDialog gdlg( gradient_ );

        if ( gdlg.exec() == QDialog::Accepted ) {
                setGradient( gdlg.gradient() );
                emit gradientChanged( gradient_ );
        }
}
