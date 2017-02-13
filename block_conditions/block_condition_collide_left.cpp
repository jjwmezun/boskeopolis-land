



// Name
//===================================
//
// BlockConditionCollideLeft
//


// DEPENDENCIES
//===================================

    #include "block.h"
    #include "block_condition_collide_left.h"
    #include "collision.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockConditionCollideLeft::BlockConditionCollideLeft() {};

    BlockConditionCollideLeft::~BlockConditionCollideLeft() {};

    bool BlockConditionCollideLeft::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
    {
        return collision.collideLeft();
    };

