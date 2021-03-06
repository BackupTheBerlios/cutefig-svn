 
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

#include "bezierspline.h"
#include "outputbackend.h"
#include "geometry.h"

#include <QPainter>
#include <QPainterPath>

#include <cmath>

//! A helper class to find subpaths to add to BezierSpline::painterPath_.
class BezierSpline::PathFinder 
{
public:
        explicit PathFinder( const BezierSpline& bs );

        //! returns true if there is another QPainterPath to be added.
        bool findNext();

        //! returns the QPainterPath to be added.
        QPainterPath currentPath() const { return currentPath_; }
        
private:
        const BezierSpline& bs_;

        QPainterPath currentPath_;

        int max_;
        int currentIndex_;
};

BezierSpline::PathFinder::PathFinder( const BezierSpline& bs )
        : bs_( bs ),
          currentPath_( bs.points_.first() ),
          max_( bs.points_.size() - 1 ),
          currentIndex_( 0 )
{
}

bool BezierSpline::PathFinder::findNext() 
{
        currentPath_ = QPainterPath( currentPath_.currentPosition() );

        ++currentIndex_;

        if ( currentIndex_ + 1 < max_ ) {
                currentPath_.cubicTo( bs_.points_[currentIndex_],
                                      bs_.points_[currentIndex_+1],
                                      bs_.points_[currentIndex_+2] );
                currentIndex_ += 2;
                return true;
        }

        if ( currentIndex_ < max_ ) {
                currentPath_.quadTo( bs_.points_[currentIndex_], bs_.points_.last() );
                return true;
        }
        
        return false;
}

BezierSpline::BezierSpline()
        : DrawObject(),
          finished_( false ),
          oppositeDirective_( 0 )
{}

BezierSpline::BezierSpline( const BezierSpline* other )
        : DrawObject( other ),
          finished_( true ),
          oppositeDirective_( 0 )
{
}

                
void BezierSpline::outputToBackend( OutputBackend* ob ) const
{
        ob->outputBezierSpline( this );
}

void BezierSpline::setupPainterPath()
{
        painterPath_ = QPainterPath( points_.first() );
        
        PathFinder pf( *this );
        while ( pf.findNext() )
                painterPath_.addPath( pf.currentPath() );
}

void BezierSpline::cursorMovePrivate( const QPointF& pos )
{
        QPointF diff = pos - points_[currentPointIndex()];
        
        if ( oppositeDirective_ > 0 ) 
                points_[oppositeDirective_] -= diff;
        else if ( oppositeDirective_ < 0 ) {
                points_[currentPointIndex()+1] += diff;
                points_[currentPointIndex()-1] += diff;
        }
}


void BezierSpline::passPointFlag( Fig::PointFlags f )
{
        if ( f & Fig::Final )
                finished_ = true;
}

int BezierSpline::nextPointIndex()
{
        if ( finished_ ) {
                int s = points_.size() - 4;
                if ( s > 0 && (s % 3==1) ) {
                        points_.pop_back();
                        oppositeDirective_ = 0;
                        setupPainterPath();
                }
                
                return -1;
        }

        int s = points_.size();
        if ( !(s % 3) ) {
                points_.insert( --s, points_.last() );
                oppositeDirective_ = s;
        } else
                oppositeDirective_ = 0;

        points_ << points_.last();
        
        return points_.size() - 1;
}

/*! 
 */
void BezierSpline::setCurrentPointIndex( int i )
{
        DrawObject::setCurrentPointIndex( i );
        
        if ( (i < 2) || (i == points_.size()-1) ) {
                oppositeDirective_ = 0;
                return;
        }

        if ( (i%3 == 0) ) {
                oppositeDirective_ = -1;
                return;
        }
        
        if ( (i-1) % 3 )
                oppositeDirective_ = i + 2;
        else
                oppositeDirective_ = i - 2;
        
        if ( oppositeDirective_ >= points_.size() )
                oppositeDirective_ = 0;
}

void BezierSpline::drawMetaData( QPainter* p ) const
{
        int max = points_.size() - 1;

        int i = 0;

        while ( i < max ) {
                p->drawLine( points_[i], points_[i+1] );

                if ( i%3 )
                        ++i;
                else
                        i+=2;
        }
}
