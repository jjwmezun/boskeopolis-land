


// Name
//===================================
//
// BlockComponentDropping
//


// DEPENDENCIES
//===================================

    #include "block.h"
    #include "block_component_dropping.h"
    #include <iostream>


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockComponentDropping::BlockComponentDropping() {};

    BlockComponentDropping::~BlockComponentDropping() {};

    void BlockComponentDropping::interact( Collision& collision, Sprite& sprite, Block& block, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
    {
        //block.hit_box_.y+=1000;
    };
