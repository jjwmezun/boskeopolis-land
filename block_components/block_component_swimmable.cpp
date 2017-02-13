


// Name
//===================================
//
// BlockComponentSwimmable
//


// DEPENDENCIES
//===================================

    #include "block_component_swimmable.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockComponentSwimmable::BlockComponentSwimmable() {};

    BlockComponentSwimmable::~BlockComponentSwimmable() {};

    void BlockComponentSwimmable::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
    {
        sprite.swim();
    };
