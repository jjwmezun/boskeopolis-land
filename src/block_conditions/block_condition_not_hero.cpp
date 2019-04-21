#include "block_condition_not_hero.hpp"
#include "sprite.hpp"

BlockConditionNotHero::BlockConditionNotHero() {};

BlockConditionNotHero::~BlockConditionNotHero() {};

bool BlockConditionNotHero::condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const
{
	return !sprite.hasType( Sprite::SpriteType::HERO );
};
