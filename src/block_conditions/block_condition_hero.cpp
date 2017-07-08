#include "block_condition_hero.h"
#include "sprite.h"

BlockConditionHero::BlockConditionHero() {};

BlockConditionHero::~BlockConditionHero() {};

bool BlockConditionHero::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return sprite.hasType( Sprite::SpriteType::HERO );
};

