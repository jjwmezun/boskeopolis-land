



// Name
//===================================
//
// AvoidMoneyGoal
//


// DEPENDENCIES
//===================================

    #include "avoid_money_goal.h"
    #include "event_system.h"
    #include "inventory_level.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    AvoidMoneyGoal::AvoidMoneyGoal()
    :
        Goal( "¡Don't touch any money!", GoalType::AVOID_MONEY )
    {};

    AvoidMoneyGoal::~AvoidMoneyGoal() {};

    void AvoidMoneyGoal::update( SpriteSystem& sprites, InventoryLevel& inventory, EventSystem& events, const Input& input )
    {
        if ( inventory.funds() > 0 )
            events.failed_ = true;
    };
