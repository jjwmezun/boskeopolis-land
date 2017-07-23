#include "block_condition_hero.hpp"
#include "sprite.hpp"

BlockConditionHero::BlockConditionHero() {};

BlockConditionHero::~BlockConditionHero() {};

bool BlockConditionHero::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return sprite.hasType( Sprite::SpriteType::HERO );
};

