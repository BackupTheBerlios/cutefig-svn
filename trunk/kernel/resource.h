

template<class T> class Resource
{
        enum Flags { Invalid = 0x00, InFigure = 0x01, BuiltIn = 0x02, InLib = 0x04 };

public:
        Resource<T>() : flags_( Invalid ) {}
        static Resource<T> makeFigureResource( T data ) { return Resource<T>( data, InFigure ); }
        static Resource<T> makeLibResource( T data ) { return Resource<T>( data, InLib ); }

        friend class ResLibInit;
        
        const T& getData() const { return data_; }
        T* data();
        
        bool isBuiltIn() const { return flags_ & BuiltIn; }
        bool isInLib() const { return flags_ & InLib; }
        bool isInFigure() const { return flags_ & InFigure; }

        operator bool () const { return !flags_; }
        
private:
        Resource<T>( T data, Flags flags ) : flags_( flags ), data_( data ) {}
        static Resource<T> makeBuiltInResource( T data ) { return Resource<T>( data, BuiltIn ); }

        Flags flags_;

        T data_;
};

