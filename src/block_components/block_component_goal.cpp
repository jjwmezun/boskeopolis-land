#include "block_component_goal.h"
#include "event_system.h"

BlockComponentGoal::BlockComponentGoal() {};

BlockComponentGoal::~BlockComponentGoal() {};

void BlockComponentGoal::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{
	events.win();
};
