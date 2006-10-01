 
/*****************************************************************************
**
**  $Id$
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

#ifndef editdialog_h
#define editdialog_h

#include <QDialog>

class QVBoxLayout;
class QPushButton;


//! Provides a dialog with three standard buttons at the bottom
/*! This class can be used as a base for any dialog to edit any kind
 *  of data structures. At the bottom there are the three standard
 *  buttons "Ok", "Cancel" and "Reset".
 *
 *  
 *  \section Interface
 *  
 *  To implement a dialog to edit a data structure you need to do the
 *  following steps:
 *
 *     -# Put the widgets you need into a QLayout and pass this QLayout to
 *        the EditDialog using the protected routine takeLayout(). If you need
 *        only one widget you can pass your widget by takeWidget().
 *     -# connect the signals of your widgets that indicate a change in
 *        the data to EditDialog's protected slot noticeChange().
 *     -# reimplement setupInitialValues() to set the widgets to the initial
 *        values.
 *     -# reimplement commitChanges() to set the change data structure to
 *        the current values of the widgets
 *     -# reimplement doReset() to reset the data structure to its initial
 *        values.   
 *
 *  The return value of QDialog::exec() will be QDialog::Rejected if
 *  the user clicks on the "Cancel" button or hits Escape. If the user
 *  clicks "Ok" or hits the Return/Enter it will allways be
 *  QDialog::Accepted no matter if the data structure has actually
 *  been changed before. To check whether the user made any changes
 *  before accepting the dialog you can check editeeChanged().
 *
 *  In case you want to be notified if the user makes any changes you
 *  can connect to the signal changeHappened().
 *
 *
 *  \section impl Implementation remarks
 *
 *  The functions setupInitialValues() and commitChanges() change
 *  values of the widgets. In order to avoid infinite loops we use the
 *  attribute #blindForChanges_. If blindForChanges_ is true the
 *  routine noticeChanges will do nothing. So the functions changing
 *  values are only to be called by the EditDialog class itself so
 *  that the proper setting of blindForChanges_ can be ensured.
 *
 *  The function doReset() only needs to restore the initial data
 *  structure. When the user clicks on "Reset" the private slot
 *  resetChanges() will call doReset() and then setupInitialValues().
 *  Note that doReset() is also called if the user rejects the dialog
 *  and has not changed anything before.
 *
 *  The initialising function setupInitialValues is called by the
 *  private slot init() which is called by the delayed initialisation
 *  idiom described in http://www.codeskipper.org/ using a
 *  QTimer::singleShot() in EditDialog::EditDialog()
 */

class EditDialog : public QDialog
{
        Q_OBJECT
public:
        EditDialog( QWidget* parent = 0 );

        //! Can be used to check if the data structure has been changed befor QDialog::accept().
        bool editeeChanged() const { return changed_; }

signals:
        //! Emitted when the data structure is changed or resetted. 
        void changeHappened();

        
protected:
        //! Puts the passed layout into the dialogLayout.
        void takeLayout( QLayout* layout, int stretch = 0 );
        void takeWidget( QWidget* widget, int stretch = 0 );

        //! To be reimplemented to reset the data structure.
        virtual void doReset() = 0;

        //! To be reimplemented to update the data structure. 
        virtual void commitChanges() = 0;

        //! To be reimplemented to set the widgets to the initial values.
        virtual void setupInitialValues() = 0;

protected slots:
        void resetChanges();
        void noticeChange();
        void reject();

private slots:
        void initialize();
        
        
private:
        QVBoxLayout* dialogLayout_;

        QPushButton* reset_;

        bool changed_;
        
        bool blindForChanges_;
};

#endif
