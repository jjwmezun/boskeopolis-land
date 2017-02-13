

// Name
//===================================
//
// BlockComponentMessage
//


// DEPENDENCIES
//===================================

    #include "block_component_message.h"
    #include "event_system.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockComponentMessage::BlockComponentMessage() {};

    BlockComponentMessage::~BlockComponentMessage() {};

    void BlockComponentMessage::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
    {
        events.message_ = true;
    };
