#include "block_condition_hero.hpp"
#include "sprite.hpp"

BlockConditionHero::BlockConditionHero() {};

BlockConditionHero::~BlockConditionHero() {};

bool BlockConditionHero::condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events )
{
	return sprite.hasType( Sprite::SpriteType::HERO );
};

