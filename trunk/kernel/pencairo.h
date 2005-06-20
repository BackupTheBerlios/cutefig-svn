 
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

#ifndef pen_h
#define pen_h

#include <qcolor.h>
#include <list>
#include <QString>

class QPainter;
class DrawObject;

template<class Resource> class ResLib;
typedef std::list<double> Dashes;
typedef ResLib<Dashes> DashesLib;

class Pen
{
public:

        enum CapStyle {
                FlatCap,
                RoundCap,
                SquareCap
        };
        
        enum JoinStyle {
                MiterJoin,
                RoundJoin,
                BevelJoin
        };
        
//        friend class QPainter;
//        friend class DrawObject;

        Pen();
        Pen( double pw, const QString& dashKey, Dashes* dsh, double dofs,
             CapStyle cs, JoinStyle js, double ml,
             const QColor& c );
        
        ~Pen() { };
        
        void setColor( const QColor& c ) { color_ = c; };
        void setLineWidth( const double& lw ) { lineWidth_ = lw; };
        bool setDashes( const QString& dashString );
        void setCapStyle( const CapStyle& cs ) { capStyle_ = cs; };
        void setJoinStyle( const JoinStyle& js ) { joinStyle_ = js; };
        void setMiterLimit( const double& ml ) { miterLimit_ = ml; };

        QColor color() const { return color_; };
        double lineWidth() const { return lineWidth_; };
        Dashes* dashes() const { return dashes_; };
        double offset() const { return offset_; };
        QString dashKey() const { return dashKey_; };
        CapStyle capStyle() const { return capStyle_; };
        JoinStyle joinStyle() const { return joinStyle_; };
        double miterLimit() const { return miterLimit_; };

private:
        QColor color_;
        double lineWidth_;
        
        QString dashKey_;
        Dashes* dashes_;
        double offset_;

        CapStyle capStyle_;
        JoinStyle joinStyle_;
        double miterLimit_;
};

#endif
