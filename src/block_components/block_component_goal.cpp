#include "block_component_goal.hpp"
#include "event_system.hpp"

BlockComponentGoal::BlockComponentGoal() {};

BlockComponentGoal::~BlockComponentGoal() {};

void BlockComponentGoal::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks )
{
	events.win();
};
