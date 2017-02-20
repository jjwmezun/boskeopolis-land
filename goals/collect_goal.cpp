

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

    CollectGoal::CollectGoal( int amount_needed, std::string message )
    :
        Goal( collectMessage( amount_needed, message ), GoalType::COLLECT, amount_needed )
    {};

    CollectGoal::~CollectGoal() {};

    void CollectGoal::update( SpriteSystem& sprites, InventoryLevel& inventory, EventSystem& events, Input& input )
    {
        if ( inventory.funds() >= value() )
            events.won_ = true;
    };

	const std::string CollectGoal::collectMessage( int amount_needed, std::string message ) const
	{
		return ( message.compare( "" ) == 0 ) ? (std::string)"¡Collect " + Text::formatNumCommas( Text::stringifyNum( amount_needed ) ) + (std::string)"\t!" : message;
	};