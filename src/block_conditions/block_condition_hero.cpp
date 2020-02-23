#include "block_condition_hero.hpp"
#include "sprite.hpp"

BlockConditionHero::BlockConditionHero() {};

BlockConditionHero::~BlockConditionHero() {};

bool BlockConditionHero::condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const
{
	return sprite.hasType( Sprite::SpriteType::HERO );
};

