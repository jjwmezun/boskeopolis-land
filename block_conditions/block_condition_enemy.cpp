




// Name
//===================================
//
// BlockConditionEnemy
//


// DEPENDENCIES
//===================================

    #include "block_condition_enemy.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockConditionEnemy::BlockConditionEnemy() {};

    BlockConditionEnemy::~BlockConditionEnemy() {};

    bool BlockConditionEnemy::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
    {
        return sprite.hasType( Sprite::SpriteType::ENEMY );
    };

