


// Name
//===================================
//
// BlockConditionCollideTop
//


// DEPENDENCIES
//===================================

    #include "block.h"
    #include "block_condition_collide_top.h"
    #include "collision.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockConditionCollideTop::BlockConditionCollideTop() {};

    BlockConditionCollideTop::~BlockConditionCollideTop() {};

    bool BlockConditionCollideTop::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
    {
        return sprite.collideTopOnly( collision, block );
    };

