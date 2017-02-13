

// Name
//===================================
//
// CollectGoal
//


// DEPENDENCIES
//===================================

    #include "collect_goal.h"
    #include "event_system.h"
    #include "inventory_level.h"
    #include "text.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    CollectGoal::CollectGoal( int amount_needed )
    :
        Goal( (std::string)"¡Collect " + Text::formatNumCommas( Text::stringifyNum( amount_needed ) ) + (std::string)"\t!", GoalType::COLLECT, amount_needed )
    {
        //message_ = ;
    };

    CollectGoal::~CollectGoal() {};

    void CollectGoal::update( SpriteSystem& sprites, InventoryLevel& inventory, EventSystem& events, Input& input )
    {
        if ( inventory.funds() >= value() )
            events.won_ = true;
    };
