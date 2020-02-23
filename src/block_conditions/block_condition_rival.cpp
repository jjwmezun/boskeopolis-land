#include "block_condition_rival.hpp"
#include "sprite.hpp"

BlockConditionRival::BlockConditionRival() {};

BlockConditionRival::~BlockConditionRival() {};

bool BlockConditionRival::condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const
{
	return sprite.hasType( Sprite::SpriteType::RIVAL );
};

