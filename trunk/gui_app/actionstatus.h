 
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

#ifndef actionstatus_h
#define actionstatus_h

#include "fig.h"

#include <QString>
#include <QMap>
#include <QList>


//! Information that an InteractiveAction can display to the user when active.
/*! As CuteFig makes big use of keyboard modifiers to determine in
 *  which way an InteractiveAction is carried out, we need a way to
 *  display to the user which modifier and which mouse button will
 *  have which effect at a particular time. We store this information
 *  an an ActionStatus that is then passed around by reference in the
 *  signal InteractiveAction::statusChanged().
 *
 *  
 *  \section Interface
 *
 *  The interface is the following: The information that depends on
 *  keyboard modifiers is stored in the class Information. A reference
 *  to the instance of Information corresponding to the current
 *  Qt::KeyboardModifiers. Those can be set and read by usual setters
 *  and getters.
 *
 *  The current status of the InteractiveAction (e.g. object moved by
 *  dx, dy) is stored outside of the modifier dependend Information.
 *
 *
 *  \section Implementation
 *
 *  To avoid the inconvenience of supplying the modifier dependend
 *  Information on every
 *  InteractiveAction::modifierChange(). Therefore those Informations
 *  are stored in infos_ and can be looked up by infoMap_ which
 *  resolves Qt::KeyboardModifiers to a pointer to Information.
 */
class ActionStatus 
{
public:
        friend class CanvasView;
        
        //! Contains the modifier dependend information for ActionStatus.
        /*! Information can be filled by setters that return
         *  refenrences to the Information itself. That way we can
         *  fill the Information without putting it into a variable just by
         *
         *  setInformation( Information().setHelp("helptext").setLeft("left button")....
         */
        class Information 
        {
        public:
                Information();
                
                QString help() const   { return help_; }
                QString left() const   { return left_; }
                QString mid() const    { return mid_; }
                QString right() const  { return right_; }

                Information& setLeft  ( const QString& l ) { left_   = l; return *this; }
                Information& setMid   ( const QString& m ) { mid_    = m; return *this; }
                Information& setRight ( const QString& r ) { right_  = r; return *this; }
                Information& setHelp  ( const QString& h ) { help_   = h; return *this; }
                
        private:
                QString  help_, left_, mid_, right_;
        };

        ActionStatus();

        //! clears the whole thing and fills the infoMap_
        void clear();
        
        const Information& information() const;

        void setInformation( const Fig::PointFlags& f,
                             const QString& b, const QString& h=QString() );

        Qt::KeyboardModifiers modifiers() const { return modifiers_; }
        void setModifiers( Qt::KeyboardModifiers mods ) { modifiers_ = mods; }

        void setStatus( const QString& s ) { status_ = s; }
        const QString& status() const { return status_; }
        
private:        
        void setInformation( Qt::KeyboardModifiers mods, const Information& info );
        void setInformation( const Information& info );

        QMap<Qt::KeyboardModifiers, Information*> infoMap_;
        Qt::KeyboardModifiers modifiers_;

        QList<Information> infos_;

        QString status_;
        
        static QString tr( const char* s );
};


#endif
