#include "block_condition_bull.hpp"
#include "sprite.hpp"

BlockConditionBull::BlockConditionBull() {};

BlockConditionBull::~BlockConditionBull() {};

bool BlockConditionBull::condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const
{
	return sprite.hasType( Sprite::SpriteType::BULL );
};

