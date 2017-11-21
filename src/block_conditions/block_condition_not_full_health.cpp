#include "block_condition_not_full_health.hpp"
#include "health.hpp"

BlockConditionNotFullHealth::BlockConditionNotFullHealth() {};
BlockConditionNotFullHealth::~BlockConditionNotFullHealth() {};

bool BlockConditionNotFullHealth::condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health )
{
	return health.hp() < health.maxHP();
};
