


// Name
//===================================
//
// BlockComponentGoal
//


// DEPENDENCIES
//===================================

    #include "block_component_goal.h"
    #include "event_system.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockComponentGoal::BlockComponentGoal() {};

    BlockComponentGoal::~BlockComponentGoal() {};

    void BlockComponentGoal::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
    {
        events.won_ = true;
    };
