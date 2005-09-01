#ifndef mouseeventhandler_h
#define mouseeventhandler_h

class QMouseEvent;

class AbstractMouseEventDispatcher
{
public:
        virtual bool initialClick( QMouseEvent* e ) = 0;
        virtual bool finalClick( QMouseEvent* e ) = 0;
        virtual void drag( QMouseEvent* e ) = 0;
        virtual void move( QMouseEvent* e ) = 0;
};

class AbstractMouseEventHandler
{
public:
        AbstractMouseEventHandler( AbstractMouseEventDispatcher* dp )
                : dispatcher_( dp ),
                  dragging_( false )
        {}

        virtual void mousePressEvent( QMouseEvent* e ) = 0;
        virtual void mouseMoveEvent( QMouseEvent* e );
        virtual void mouseReleaseEvent( QMouseEvent* e ) = 0;

protected:
        AbstractMouseEventDispatcher* dispatcher_;
        bool dragging_;
};

class ClickMouseEventHandler : public AbstractMouseEventHandler
{
public:
        ClickMouseEventHandler( AbstractMouseEventDispatcher* dp )
                : AbstractMouseEventHandler( dp )
        {}
        
        virtual void mousePressEvent( QMouseEvent* ) {}
        virtual void mouseReleaseEvent( QMouseEvent* e );
};

class DragMouseEventHandler : public AbstractMouseEventHandler
{
public:
        DragMouseEventHandler( AbstractMouseEventDispatcher* dp )
                : AbstractMouseEventHandler( dp )
        {}
        
        virtual void mousePressEvent( QMouseEvent* e );
        virtual void mouseReleaseEvent( QMouseEvent* e );
};


#endif
