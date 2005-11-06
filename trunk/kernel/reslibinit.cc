 
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


#include "reslibinit.h"

#include "reslib.h"
#include "typedefs.h"
#include "strokelib.h"
#include "gradient.h"
#include "parser.h"

void ResLibInit::init()
{
        static bool called = false;
        if ( called )
                return;
        
        initDashes();
        initGradients();
        called = true;
}

void ResLibInit::initDashes()
{
        DashesLib& dl = DashesLib::instance();

        insert( dl, "S",               QVector<double>() );
        insert( dl, "Dot",             Parser::parseDashLine("1 3") );
        insert( dl, "Dash",            Parser::parseDashLine("3 3") );
        insert( dl, "DashDot",         Parser::parseDashLine("3 3 1 3") );
        insert( dl, "DashDotDot",      Parser::parseDashLine("3 3 1 3 1 3") );
        insert( dl, "DashDotDotDot",   Parser::parseDashLine("3 3 1 3 1 3 1 3") );
        insert( dl, "DashDashDot",     Parser::parseDashLine("3 3 3 3 1 3") );
        insert( dl, "DashDashDashDot", Parser::parseDashLine("3 3 3 3 3 3 1 3") );
        insert( dl, "DashDashDotDot",  Parser::parseDashLine("3 3 3 3 1 3 1 3") );
}

void ResLibInit::initGradients()
{
        Gradient defaultGrad( Gradient::Linear, QPointF(0,0), QPointF(1,1) );
        Gradient linGrad( Gradient::Linear, QPointF(0,0), QPointF(1,1) );
        Gradient radGrad( Gradient::Radial, QPointF(.5,.5), QPointF(.75,.25) ) ;
        radGrad.setRadius( .3 );
        
        defaultGrad.setColorAt( 0, Qt::black );
        defaultGrad.setColorAt( 1, Qt::white );
        
        linGrad.setColorAt( 0, Qt::red );
        linGrad.setColorAt( 1, Qt::blue );

        radGrad.setColorAt( 0, Qt::red );
        radGrad.setColorAt( 1, Qt::blue );

        
        StrokeLib& sl = StrokeLib::instance();
        
        insert( sl, "red", Stroke( ResourceKey::builtIn( "red" ), Qt::red ) );
        insert( sl, "defaultGradient",
                Stroke( ResourceKey::builtIn("defaultGradient"), defaultGrad ) );
        insert( sl, "linearBuiltIn",
                Stroke( ResourceKey::builtIn("linearBuiltIn"),   linGrad ) );
        insert( sl, "radialBuiltIn",
                Stroke( ResourceKey::builtIn("radialBuiltIn"),   radGrad ) );
}
