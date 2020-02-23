#include "block_component_goal.hpp"
#include "event_system.hpp"

BlockComponentGoal::BlockComponentGoal() {};

BlockComponentGoal::~BlockComponentGoal() {};

void BlockComponentGoal::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	events.win();
};
