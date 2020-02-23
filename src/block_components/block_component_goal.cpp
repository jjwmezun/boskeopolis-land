#include "block_component_goal.hpp"
#include "event_system.hpp"
#include "level_state.hpp"

BlockComponentGoal::BlockComponentGoal() {};

BlockComponentGoal::~BlockComponentGoal() {};

void BlockComponentGoal::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	level_state.events().win();
};
