 
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

#ifndef bezierspline_h
#define bezierspline_h

#include "drawobject.h"

//! Draws connected bezier curves
/*! To draw itself a BezierSpline needs at least three points. As long
 *  as there are enough points left, a cubic bezier curve is drawn. If
 *  in the end there are only two more points left, a quadratic bezier
 *  curve is added. The case that in the end there is only one point
 *  left should not occur, i.e. BezierSpline has to take care that it
 *  does not.
 *
 *  
 *  \section pointkinds Kinds of Points
 *  BezierSpline knows two different kinds of points.
 *  
 *     - Nodes. These are points \em on the spline. Mathematically
 *       spoken they are start or end points of subsplines.
 *
 *     - Directives. These define tangents. The line defined by a
 *       directive and its node is the tangent on the spline in the
 *       node.
 *
 *  So the first and the last point of a BezierSpline are always
 *  nodes. Moreover the successor and the preden? of a node are always
 *  directives. Each directive is bound to exactly one node. Every
 *  node exept the first and the last one has two directives.
 *
 *  
 *  \section transitions Transitions between two subsplines
 *  
 *  A transition between two subsplines can be either \em smooth or
 *  \em angular. Smooth means that both subsplines are hitting each
 *  other at their connecting node at the same angle. That means that
 *  the vector pointing from the node to one directive is the same one
 *  that points from the node to the other directive exept the sign.
 *
 *  By now all transitions are smooth. So if one point is moved by the
 *  user, other points might be moved as well. This is the case if
 *
 *      - a directive is moved. Then the other directive of the node has
 *        to be moved in the opposite direction.
 *
 *      - a node, exept the first and the last, is moved. Then both
 *        directives have to be moved into the same direction. 
 *
 *  It is not yet decided whether it will be possible to make the
 *  transitions between two curves angular but probably so. 
 */
class BezierSpline : public DrawObject
{
        Q_OBJECT
        DRAW_OBJECT( "bezierspline", "&Bezier spline" );

        // A helper class to find subpaths 
        class PathFinder;
        
public:
        explicit BezierSpline( Figure* parent );
        BezierSpline( const BezierSpline* other );

        friend class PathFinder;

        virtual int minPoints() const { return 3; }
        
        virtual void outputToBackend( OutputBackend* ob ) const;

        virtual void setCurrentPointIndex( int i );

        void drawMetaData( QPainter* p ) const;
        
protected:
	virtual void cursorMovePrivate( const QPointF& pos );
        virtual void setupPainterPath();
        virtual void passPointFlag( Fig::PointFlags f );
        virtual int nextPointIndex();

        
private:
        bool finished_;

        //! Indicates whether and which other points have to be moved.
        /*! There are three cases that can occur.
         *
         *      - There is no other point to be moved. Indicated by a
         *        value of 0. This is the case when the first or the
         *        last node is moved
         *      
         *      - There is one other point to be moved. In this case
         *        the value is the index of the point that is to be
         *        moved. This is the case when a directive is moded.
         *
         *      - There are are two other points to be moved. Indicated
         *        by a value of -1. This is the case when a node in the
         *        middle is moved. 
         */
        int oppositeDirective_;
};

#endif
