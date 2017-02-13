



// Name
//===================================
//
// BlockConditionHero
//


// DEPENDENCIES
//===================================

    #include "block_condition_hero.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BlockConditionHero::BlockConditionHero() {};

    BlockConditionHero::~BlockConditionHero() {};

    bool BlockConditionHero::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
    {
        return sprite.hasType( Sprite::SpriteType::HERO );
    };

