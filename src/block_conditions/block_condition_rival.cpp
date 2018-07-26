#include "block_condition_rival.hpp"
#include "sprite.hpp"

BlockConditionRival::BlockConditionRival() {};

BlockConditionRival::~BlockConditionRival() {};

bool BlockConditionRival::condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const
{
	return sprite.hasType( Sprite::SpriteType::RIVAL );
};

