




// Name
//===================================
//
// BlockConditionNotDucking
//


// DEPENDENCIES
//===================================

    #include "block_condition_not_ducking.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockConditionNotDucking::BlockConditionNotDucking() {};

    BlockConditionNotDucking::~BlockConditionNotDucking() {};

    bool BlockConditionNotDucking::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
    {
        return !sprite.isDucking();
    };

