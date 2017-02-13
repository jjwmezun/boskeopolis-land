





// Name
//===================================
//
// BlockConditionRival
//


// DEPENDENCIES
//===================================

    #include "block_condition_rival.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockConditionRival::BlockConditionRival() {};

    BlockConditionRival::~BlockConditionRival() {};

    bool BlockConditionRival::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
    {
        return sprite.hasType( Sprite::SpriteType::RIVAL );
    };

