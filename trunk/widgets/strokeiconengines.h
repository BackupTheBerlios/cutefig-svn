 
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

#ifndef strokeiconengines_h
#define strokeiconengines_h

#include <QIconEngine>

#include "stroke.h"

class Gradient;

class AbstractStrokeIconEngine : public QIconEngine 
{
public:
        AbstractStrokeIconEngine() : QIconEngine() {}
        ~AbstractStrokeIconEngine() {}

        virtual void paint( QPainter* p, const QRect& r, QIcon::Mode md, QIcon::State );

protected:
        virtual void paintForeground( QPainter* p, const QRect& rect ) = 0;
};

class ColorIconEngine : public AbstractStrokeIconEngine
{
public:
        ColorIconEngine( const QColor& c ) : AbstractStrokeIconEngine(), color_( c ) {}
        ~ColorIconEngine() {}

protected:
        virtual void paintForeground( QPainter* p, const QRect& r );

private:
        const QColor& color_;
};

class GradientIconEngine : public AbstractStrokeIconEngine
{
public:
        GradientIconEngine( const Gradient& g ) : AbstractStrokeIconEngine(), gradient_( g ) {}
        ~GradientIconEngine() {}

protected:
        virtual void paintForeground( QPainter* p, const QRect& r );
        
private:
        const Gradient& gradient_;
};

class StrokeIconEngine : public AbstractStrokeIconEngine
{
public:
        StrokeIconEngine( const Stroke& s ) : AbstractStrokeIconEngine(), stroke_( s ) {}
        ~StrokeIconEngine() {}

protected:
        virtual void paintForeground( QPainter* p, const QRect& r );

private:
        Stroke stroke_;
};

#endif
