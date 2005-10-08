#ifndef strokelib_h
#define strokelib_h

#include "reslib.h"
#include "stroke.h"
#include "gradient.h"
#include "typedefs.h"

#include <QColor>

// template<> class ResLib<QColor> 
// {
//         friend class ResLib<Stroke>;
// };


template<> class ResLib<Stroke> 
{
public:
        friend class ResLibInit;

        static ResLib<Stroke>& instance()
        {
                static ResLib<Stroke> inst;
                return inst;
        };
        
        void insert( const ResourceKey& key, const Stroke& stroke )
        {
                if ( key.isBuiltIn() )
                        return;
                
                switch ( stroke.type() ) {
                    case Stroke::sColor:
                            colorLib.insert( key, stroke.color() ); break;
                    case Stroke::sGradient:
                            gradLib.insert( key, stroke.gradient() ); break;
                    default: break;
                }

                keys_ << key;
        }

        bool remove( const ResourceKey& key )
        {
                if ( key.isBuiltIn() ) 
                        return false;

                int r = colorLib.remove( key );
                if ( r )
                        return r;

                gradLib.remove( key );

                return keys_.removeAll( key );
        }
        
        const Stroke operator[]( const ResourceKey& key ) const
        {
                if ( colorLib.contains( key ) )
                        return colorLib[key];
                if ( gradLib.contains( key ) )
                        return gradLib[key];

                return Stroke();
        }
        
//        const ResourceKey key( const Resource& data ) const { return map_[data]; }
        bool contains( const ResourceKey& key ) const
        {
                return keys_.contains( key );
        }               

        const ResourceKeyList& keys() const { return keys_; }

//        QList<Resource> resources() const { return map_.values(); }
        
private:
        ResLib<Stroke>() : keys_() {}
        ResLib<Stroke>( const ResLib<Stroke>& ) {}
        
        void insertBuiltIn( const ResourceKey& key, const Stroke& data )
        {
                switch ( data.type() ) {
                    case Stroke::sColor:
                            colorLib.insertBuiltIn( key, data.color() ); break;
                    case Stroke::sGradient:
                            gradLib.insertBuiltIn( key, data.gradient() ); break;
                    default: break;
                }

                keys_ << key;
       }
        
//        QMap<ResourceKey, Resource> map_;
        ResourceKeyList keys_;
        
        static ColorLib& colorLib;
        static GradLib& gradLib;
};


#endif
