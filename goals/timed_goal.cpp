


// Name
//===================================
//
// TimedGoal
//


// DEPENDENCIES
//===================================

    #include "timed_goal.h"
    #include "event_system.h"
    #include "inventory_level.h"
    #include "text.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    TimedGoal::TimedGoal( int time_limit )
    :
        Goal( "¡Don't let time run out!", GoalType::TIMED, time_limit )
    {};

    TimedGoal::~TimedGoal() {};

    void TimedGoal::update( SpriteSystem& sprites, InventoryLevel& inventory, EventSystem& events, Input& input )
    {
        if ( inventory.clockTime() >= value() )
            events.failed_ = true;
    };
