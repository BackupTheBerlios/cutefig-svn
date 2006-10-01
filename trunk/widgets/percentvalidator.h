 
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

#ifndef percentvalidatior_h
#define percentvalidatior_h

#include <QIntValidator>

//! A subclass of QIntValidator, that validates values like "200 %"
/*! Validates int values from bottom() to top(), ensuring that bottom() >= 1.
 *  validate() will ensure that a the input string endsWith(" %").
 *
 *  The value that has been validated recently is readable by
 *  lastValidated(). 
 */
class PercentValidator : public QIntValidator
{
        Q_OBJECT
public:
        PercentValidator( QObject* parent );

        //! \reimp QIntValidator::setRange() ensuring min >= 1
        void setRange( int min, int max );
        
        virtual QValidator::State validate( QString& input, int& pos ) const;

        //! returns the value validated recently.
        /*! Note that this value is a double which represents the percentage
         *  (1.0 if the inputstring is "100 %").
         */
        double lastValidated() const;

private:
        //! the int value that has been validated recently.
        /*! Has to be mutable as QValidator::validate() is qualified
         *  const.
         */
        mutable int lastValidated_;
};

#endif
