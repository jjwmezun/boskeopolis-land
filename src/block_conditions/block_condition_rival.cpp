#include "block_condition_rival.h"
#include "sprite.h"

BlockConditionRival::BlockConditionRival() {};

BlockConditionRival::~BlockConditionRival() {};

bool BlockConditionRival::condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events )
{
	return sprite.hasType( Sprite::SpriteType::RIVAL );
};

