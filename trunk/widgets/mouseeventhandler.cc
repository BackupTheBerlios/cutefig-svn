
#include "mouseeventhandler.h"


void AbstractMouseEventHandler::mouseMoveEvent( QMouseEvent* e )
{
        if ( dragging_ )
                dispatcher_->drag( e );
        else
                dispatcher_->move( e );

}

void ClickMouseEventHandler::mouseReleaseEvent( QMouseEvent* e )
{
        if ( dragging_ ) 
                dragging_ = dispatcher_->finalClick( e );
        else 
                dragging_ = dispatcher_->initialClick( e );
}


void DragMouseEventHandler::mousePressEvent( QMouseEvent* e )
{
        dragging_ = dispatcher_->initialClick( e );
}


void DragMouseEventHandler::mouseReleaseEvent( QMouseEvent* e )
{
        if ( dragging_ )
                dispatcher_->finalClick( e );
        dragging_ = false;
}
