 
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


#include "stroke.h"
#include "gradient.h"
#include "pixmap.h"
#include "pattern.h"
#include "resourceuser.h"

#include <QPainter>
#include <QPainterPath>
#include <QColor>
#include <QBrush>

#include <QDebug>


// The following specialisations have to be defined before their instantation.
template<> void ResLib<QColor>::init()
{
}

template<> ResourceKey ResLib<QColor>::defaultKey()
{
        return ResourceKey();
}

template<>
const QString ResLib<QColor>::resourceName() { return "color"; }

template<>
bool TResourceIO<QColor>::parseResource( const QString&, QTextStream& is )
{
        is >> resource_;
        
        failed_ = is.status() != QTextStream::Ok;

        return false;
}

template<>
void TResourceIO<QColor>::outputResourceBody( const QColor& res, QTextStream& stream ) const 
{
        stream << res << " ";
}



class StrokeDataSetter 
{
public:
	virtual ~StrokeDataSetter() {}
	
	static bool setData( Stroke* st, const QString& typeString, const ResourceKey& key );

protected:
	StrokeDataSetter( const QString& typeString );
	
private:
	virtual void doSetData( Stroke*, const ResourceKey& key ) = 0;

	static QHash<QString, StrokeDataSetter*>& hash();
};



Stroke::Stroke()
        : type_( sNone ),
          resourceUser_( 0 )
{}

Stroke::Stroke( const Stroke& other )
        : type_( other.type_ ),
          resourceUser_( other.resourceUser_ ? other.resourceUser_->clone() : 0 )
{
}

Stroke::Stroke( const QColor& color )
        : type_( sColor ),
          resourceUser_( new ResourceUser<QColor>( color ) )
{}

Stroke& Stroke::operator=( const Stroke& other )
{
        if ( this == &other )
                return *this;
        
        type_ = other.type_;

        delete resourceUser_;
        
        resourceUser_ = other.resourceUser_ ? other.resourceUser_->clone() : 0;
        
        return *this;
}


Stroke::~Stroke()
{
        delete resourceUser_;
}

void Stroke::setColor( const QColor& color )
{
        type_ = sColor;

        delete resourceUser_;
        
        resourceUser_ = new ResourceUser<QColor>();
        static_cast<ResourceUser<QColor>*>( resourceUser_ )->setResource( color );
}

bool Stroke::setData( const QString& typeString, const ResourceKey& key )
{
	return StrokeDataSetter::setData( this, typeString, key );
}

QColor Stroke::color() const
{
        if ( type_ == sColor )
                return resourceUser_->resource<QColor>();
        else
                return QColor();
}


void Stroke::fillPath( const QPainterPath& path, QPainter* painter ) const
{
        switch ( type_ ) {
            case sColor:
                    painter->fillPath( path, QBrush( resourceUser_->resource<QColor>() ) );
                    break;
            case sGradient:
            {
                    QRectF r = path.boundingRect();
                    QGradient* qgrad = resourceUser_->resource<Gradient>().toQGradient( r );
                    if ( qgrad )
                            painter->fillPath( path, *qgrad );
                    delete qgrad;
                    break;
            }
            case sPixmap:
                    painter->fillPath( path, QBrush(resourceUser_->resource<Pixmap>().qpixmap()) );
                    break;
            case sPattern:
            {
                    QMatrix mat = painter->worldMatrix();
//                    QRegion clipreg = painter->clipRegion();
                    painter->setWorldMatrix( QMatrix() );
                    QPainterPath pth = mat.map( path );
//                    painter->setClipPath( path );
                    QRectF r = path.boundingRect();
                    painter->fillPath(pth,resourceUser_->resource<Pattern>().brush(r,mat));
                    painter->setWorldMatrix( mat );
//                    painter->setClipRegion( clipreg );
                    break;
            }
            case sNone:
            default: break;
        }
}

const QBrush Stroke::brush( const QRectF& rect, const QMatrix& m ) const
{
        QBrush ret;
        
        switch ( type_ ) {
            case sColor:
                    ret = resourceUser_->resource<QColor>();
                    break;
            case sGradient:
            {
                    QGradient* qgrad = resourceUser_->resource<Gradient>().toQGradient( rect );
                    if ( qgrad )
                            ret = QBrush( *qgrad );
                    delete qgrad;
                    break;
            }
            case sPixmap:
                    ret = resourceUser_->resource<Pixmap>().qpixmap();
                    break;
 	    case sPattern:
		    ret = resourceUser_->resource<Pattern>().brush( rect, m );
		    break;
            case sNone:
            default: break;
        }

        return ret;    
}

const QBrush Stroke::brush( const QPainterPath& path, const QMatrix& m ) const
{
        QRectF r;

        if ( type_ == sGradient )
                r = path.boundingRect();

        return brush( r, m );
}

const QString Stroke::typeString() const
{
	return typeHash()[type_];
}


const ResourceKey Stroke::key() const
{
        return resourceUser_ ? resourceUser_->key() : ResourceKey();
}



template<>
void Stroke::setData<NoStroke>( const ResourceKey& )
{
	type_ = sNone;

	delete resourceUser_;
	resourceUser_ = 0;
}


// StrokeDataSetter

StrokeDataSetter::StrokeDataSetter( const QString& typeString )
{
	hash()[typeString] = this;
}

QHash<QString, StrokeDataSetter*>& StrokeDataSetter::hash()
{
	static QHash<QString, StrokeDataSetter*> h;
	return h;
}


bool StrokeDataSetter::setData( Stroke* st, const QString& typeString, const ResourceKey& key )
{
	StrokeDataSetter* sds = hash()[typeString];

	if ( !sds ) 
		return false;

	sds->doSetData( st, key );

	return true;
}


template<typename StrokeType>
class TStrokeDataSetter : public StrokeDataSetter
{
public:
        TStrokeDataSetter() : StrokeDataSetter( ResLib<StrokeType>::resourceName() ) {}

private:
	virtual void doSetData( Stroke* st, const ResourceKey& key )
	{
		st->setData<StrokeType>(key);
	}
};

static TStrokeDataSetter<QColor> cds;
static TStrokeDataSetter<Gradient> gds;
static TStrokeDataSetter<Pixmap> pxds;
static TStrokeDataSetter<Pattern> ptds;




const AutoHash<Stroke::StrokeType, QString>& Stroke::typeHash()
{
	static AutoHash<StrokeType, QString> as;
	return as;
}

template<>
void AutoHash<Stroke::StrokeType, QString>::init()
{
	hash_[Stroke::sColor] = ResLib<QColor>::resourceName();
	hash_[Stroke::sGradient] = ResLib<Gradient>::resourceName();
	hash_[Stroke::sPixmap] = ResLib<Pixmap>::resourceName();
	hash_[Stroke::sPattern] = ResLib<Pattern>::resourceName();
}


template<> Stroke::StrokeType strokeType<QColor>() { return Stroke::sColor; }
template<> Stroke::StrokeType strokeType<Gradient>() { return Stroke::sGradient; }
template<> Stroke::StrokeType strokeType<Pixmap>() { return Stroke::sPixmap; }
template<> Stroke::StrokeType strokeType<Pattern>() { return Stroke::sPattern; }
