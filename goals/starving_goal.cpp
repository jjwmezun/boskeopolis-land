


// Name
//===================================
//
// StarvingGoal
//


// DEPENDENCIES
//===================================

    #include "starving_goal.h"
    #include "event_system.h"
    #include "inventory_level.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    StarvingGoal::StarvingGoal( int amount_lost_per_frame )
    :
        Goal( "¡Don't let your money run out!", GoalType::STARVING, amount_lost_per_frame )
    {};

    StarvingGoal::~StarvingGoal() {};

    void StarvingGoal::update( SpriteSystem& sprites, InventoryLevel& inventory, EventSystem& events, const Input& input )
    {
        if ( !started_ )
        {
            inventory.setFunds( 500 );
            started_ = true;
        }
        else
        {

            if ( timer_.hit() )
            {
                inventory.loseFunds( value() );

                if ( inventory.funds() <= 0 )
                    events.failed_ = true;
            }

            timer_.update();
        }
    };
