


// Name
//===================================
//
// SurviveTimeGoal
//


// DEPENDENCIES
//===================================

    #include "survive_time_goal.h"
    #include "event_system.h"
    #include "inventory_level.h"
    #include "text.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SurviveTimeGoal::SurviveTimeGoal( int wait_time, std::string message )
    :
        Goal( surviveMessage( wait_time, message ), GoalType::SURVIVE_TIME, wait_time )
    {};

    SurviveTimeGoal::~SurviveTimeGoal() {};

    void SurviveTimeGoal::update( SpriteSystem& sprites, InventoryLevel& inventory, EventSystem& events, const Input& input )
    {
        if ( inventory.clockTime() >= value() )
            events.won_ = true;
    };

	const std::string SurviveTimeGoal::surviveMessage( int wait_time, std::string message ) const
	{
		return ( message.compare( "" ) == 0 ) ? (std::string)"¡Last " + Text::formatNumCommas( Text::stringifyNum( wait_time ) ) + (std::string)" seconds without dying!" : message;
	};