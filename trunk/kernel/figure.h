 
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


#ifndef figure_h
#define figure_h

#include "typedefs.h"
#include "length.h"
#include "paper.h"

#include <QList>
#include <QObject>
#include <QSizeF>
#include <QDate>

class DrawObject;
class OutputBackend;

class QPainter;

class QPointF;
class QPoint;
class QRectF;


//! contains all DrawObject objects of a figure. It is the \e document the user is editing.
/*! The Figure is the Model in the MVC approach. It takes a pointer to
 *  a Controler by setControler() which is usually called by an
 *  instance of a Controler. The Controler tells the Figure about
 *  updates the user made. This is done by using the routines
 *  addDrawObject() and removeDrawObject().
 *
 *  To paint the figure on a view, the Figure can be passed a pointer
 *  to a QPainter by the routine drawElements(). All elements will
 *  be drawn to this painter.
 *
 *  A Figure has two ObjectLists: objectList_ and drawingLists_. The
 *  former contains all the DrawObjects of the figure including
 *  Compounds. The latter does not contain the Compouds but all
 *  children and grandchildren of the compounds unless they are
 *  compounds themselfes. This distinction is necessary because
 *  without it a Compound would not honor different depths of
 *  childObjects.
 */
class Figure : public QObject
{
Q_OBJECT

public:
        explicit Figure( QObject * parent = 0 );
        ~Figure() {};


        //! Contains the Figures meta data
        /*! All the metadata is put into one class so that it can be
         *  copied or referenced to easily in one statement.
         */
        class MetaData
        {
        public:
                friend class Figure;
                
                MetaData();
                
                double scale() const { return scale_; }
                const ResourceUser<Length>& unit() const { return unit_; }
                const ResourceUser<Paper>& paper() const { return paper_; }
                const QString& author() const { return author_; }
		bool authorToBeSaved() const { return authorToBeSaved_; }
                const QString& description() const { return description_; }

                QDate creationDate() const { return creationDate_; }
                QDate modificationDate() const { return modDate_; }
                
        private:
                double scale_;
                ResourceUser<Length> unit_;
                ResourceUser<Paper> paper_;

                QString author_;
		bool authorToBeSaved_;
                QString description_;

                QDate creationDate_;
                QDate modDate_;
        };

        //! sets the controler_ to c
        //void setControler( Controler* c ) { controler_ = c; }

        const MetaData& metaData() const { return metaData_; }
        void setMetaData( const MetaData& md ) { metaData_ = md; }
        
        //! returns the scale_ of the Figure.
        double scale() const { return metaData_.scale_; }
        void setScale( double s ) { metaData_.scale_ = s; }
        
        const Length& unit() const { return metaData_.unit_; }
        const ResourceKey& unitKey() const { return metaData_.unit_.key(); }
        void setUnit( const ResourceKey& k ) { metaData_.unit_.setResource( k ); }

        const Paper& paper() const { return metaData_.paper_; }
        const ResourceKey& paperKey() const { return metaData_.paper_.key(); } 
        void setPaper( const ResourceKey& k ) { metaData_.paper_.setResource( k ); }

        const QString& author() const { return metaData_.author(); }
        void setAuthor( const QString& a ) { metaData_.author_ = a; }

	bool authorIsToBeSaved() const { return metaData_.authorToBeSaved(); }
	void setAuthorToBeSaved( bool s ) { metaData_.authorToBeSaved_ = s; }
        
        const QString& description() const { return metaData_.description(); }
        void setDescription( const QString& d ) { metaData_.description_ = d; }

        QDate creationDate() const { return metaData_.creationDate(); }
        void setCreationDate( const QDate& d ) { metaData_.creationDate_ = d; }
        
        QDate modificationDate() const { return metaData_.modificationDate(); }
        void setModificationDate( const QDate& d ) { metaData_.modDate_ = d; }
        
        void updateModificationDate();
        
        //! adds all the DrawObjects of the ObjectList l.
        void takeDrawObjects( const ObjectList& l );

        //! adds one DrawObject
        void addDrawObject( DrawObject* o );

        //! removes a DrawObject
        void removeDrawObject( DrawObject* o );

        //! sorts the objects
        void sortObjects(); 

        //! draws all DrawObjects exept the bakups
        void drawElements( QPainter* p ) const; 

        //! returns a pointer tho the DrawObject under the point p
        DrawObject* findContainingObject( const QPointF& p  ) const;

        //! Outputs all objects to the OutputBackend ob
        void outputObjects( OutputBackend* ob ) const;

        //! returns th bounding rect of the figure
        QRectF boundingRect() const;

        //! true iff there are no DrawObjects
        bool isEmpty() const { return objectList_.isEmpty(); }
                

        //! Clears the objectList_ and deletes all containing objects.
        void clear();

        //! returns all Resources used by the Figure
        const ResourceSet usedResources() const;
        
        //! returns the objects of the figure
        const ObjectList& objects() const { return objectList_; }


        const ObjectList& objectsToBeDrawn() const { return drawingList_; }

        
private:
        void addObjectToDrawingList( DrawObject* o );
        void removeObjectFromDrawingList( DrawObject* o );
        void sortIntoDrawingList( DrawObject* o );
        
        ObjectList objectList_, drawingList_;

        MetaData metaData_;        
};

#endif
