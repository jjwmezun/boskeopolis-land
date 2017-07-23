#include "block_condition_rival.hpp"
#include "sprite.hpp"

BlockConditionRival::BlockConditionRival() {};

BlockConditionRival::~BlockConditionRival() {};

bool BlockConditionRival::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return sprite.hasType( Sprite::SpriteType::RIVAL );
};

