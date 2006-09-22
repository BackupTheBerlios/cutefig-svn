 
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

#include "actionstatus.h"
#include "pointflagscalc.h"

#include <QCoreApplication>

#include <QDebug>

/*! Calls clear() in order to fill infoHash_ to contain all valid
 *  Qt::KeyboardModifier.
 */
ActionStatus::ActionStatus()
        : infoMap_(),
          modifiers_( Qt::NoModifier ),
          infos_(),
          status_()
{
        clear();
}

void ActionStatus::setInformation( const Fig::PointFlags& f, const QString& b, const QString& h )
{
        Qt::KeyboardModifiers m = PointFlagsCalc::toKbdModifiers( f );
        Information info = *infoMap_[m];

        switch ( PointFlagsCalc::toMouseButton( f ) ) {
            case Qt::MidButton:
                    info.setMid( b ); break;
            case Qt::RightButton:
                    info.setRight( b ); break;
            default:
                    info.setLeft( b );
        }

        info.setHelp( h );

        setInformation( m, info );
}

//! Sets the information for the modifiers.
/*! The code of this function looks a bit confusing on the first
 *  view. This is due to the problem that we need to make several
 *  modifier combination resolve to the same Information. For example
 *  if one InteractiveAction only understands the Ctrl-key the same
 *  text is to be displayed for all modifier combinations that contain
 *  the Ctrl-Key. This is obtained by this function.
 */
void ActionStatus::setInformation( Qt::KeyboardModifiers mods, const Information& info )
{
        if ( mods == Qt::NoModifier ) {
                infos_.first() = info;
                return;
        }
        
        Information* oldInf = infoMap_[mods];
        
        QList<Qt::KeyboardModifiers> fMods = infoMap_.keys( oldInf );

        if ( fMods.size() == 1 ) {
                *oldInf = info;
                return;
        }

        infos_ << info;
        Information* newInf = &infos_.last();
        
        typedef QList<Qt::KeyboardModifiers>::iterator IT;
        for ( IT it = qLowerBound( fMods.begin(), fMods.end(), mods ); it != fMods.end(); ++it )
                if ( (*it & mods) )
                        infoMap_[*it] = newInf;
}

void ActionStatus::setInformation( const Information& info )
{
        setInformation( modifiers_, info );
}

const ActionStatus::Information& ActionStatus::information() const
{
	return *infoMap_[modifiers_];
}

/*! The infoMap_ needs to be filled so that every valid modifier
 *  combination actually resolves to a valid pointer to Information.
 *  So all modifier combinations are made resolve to the first item of
 *  infos_.
 */
void ActionStatus::clear()
{
        status_.clear();

        infos_.clear();
        infos_ << Information();
        
        infoMap_[Qt::NoModifier                           ] = &infos_.first();
        infoMap_[Qt::AltModifier                          ] = &infos_.first();
        infoMap_[Qt::AltModifier     | Qt::ControlModifier] = &infos_.first();
        infoMap_[Qt::AltModifier     | Qt::ShiftModifier  ] = &infos_.first();

        infoMap_[Qt::ControlModifier                      ] = &infos_.first();
        infoMap_[Qt::ControlModifier | Qt::AltModifier    ] = &infos_.first();
        infoMap_[Qt::ControlModifier | Qt::ShiftModifier  ] = &infos_.first();

        infoMap_[Qt::ShiftModifier                        ] = &infos_.first();
        infoMap_[Qt::ShiftModifier   | Qt::ControlModifier] = &infos_.first();
        infoMap_[Qt::ShiftModifier   | Qt::AltModifier    ] = &infos_.first();

        modifiers_ = Qt::NoModifier;
}

QString ActionStatus::tr( const char* s )
{
        return QCoreApplication::translate("ActionStatus", s );
}


/*! Initialises everything to QString() except the right click rection
 *  which is initialised to tr("Cancel") as this is the default
 *  behaviour.
 */
ActionStatus::Information::Information()
        : help_(), left_(), mid_(), right_( tr("Cancel") )
{
}



void ActionStatus::setModifiers( Qt::KeyboardModifiers mods )
{
	modifiers_ = mods & ( Qt::AltModifier | Qt::ShiftModifier | Qt::ControlModifier );
}
