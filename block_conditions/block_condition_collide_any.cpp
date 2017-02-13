


// Name
//===================================
//
// BlockConditionCollideAny
//


// DEPENDENCIES
//===================================

    #include "block_condition_collide_any.h"
    #include "collision.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockConditionCollideAny::BlockConditionCollideAny() {};

    BlockConditionCollideAny::~BlockConditionCollideAny() {};

    bool BlockConditionCollideAny::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
    {
        if ( collision.collideAny() )
            return true;

        return false;
    };
