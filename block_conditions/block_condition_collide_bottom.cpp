



// Name
//===================================
//
// BlockConditionCollideBottom
//


// DEPENDENCIES
//===================================

    #include "block.h"
    #include "block_condition_collide_bottom.h"
    #include "collision.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockConditionCollideBottom::BlockConditionCollideBottom() {};

    BlockConditionCollideBottom::~BlockConditionCollideBottom() {};

    bool BlockConditionCollideBottom::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
    {
        if ( sprite.collideBottomOnly( collision, block ) )
        {
            return true;
        }

        return false;
    };

