
#include "actionpushbutton.h"

ActionPushButton::ActionPushButton( QAction* action, QWidget* parent )
        : QPushButton( parent ),
          action_( action )
{
        actionChanged();
        connect( action_, SIGNAL( changed() ), this, SLOT( actionChanged() ) );
        connect( this, SIGNAL( clicked() ), action_, SLOT( trigger() ) );
}

void ActionPushButton::actionChanged()
{
        setText( action_->text() );
        setIcon( action_->icon() );
        setEnabled( action_->isEnabled() );
        setToolTip( action_->toolTip() );
//        setShortcut( action_->shortcut() );
}
