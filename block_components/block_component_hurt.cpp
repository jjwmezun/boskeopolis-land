


// Name
//===================================
//
// BlockComponentHurt
//


// DEPENDENCIES
//===================================

    #include "block_component_hurt.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockComponentHurt::BlockComponentHurt() {};

    BlockComponentHurt::~BlockComponentHurt() {};

    void BlockComponentHurt::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
    {
        sprite.hurt();
    };