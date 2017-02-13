



// Name
//===================================
//
// BlockComponentLose
//


// DEPENDENCIES
//===================================

    #include "block_component_lose.h"
    #include "event_system.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockComponentLose::BlockComponentLose() {};

    BlockComponentLose::~BlockComponentLose() {};

    void BlockComponentLose::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
    {
        events.failed_ = true;
    };
