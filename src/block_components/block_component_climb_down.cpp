#include "block_component_climb_down.hpp"
#include "event_system.hpp"

BlockComponentClimbDown::BlockComponentClimbDown() {};

BlockComponentClimbDown::~BlockComponentClimbDown() {};

void BlockComponentClimbDown::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	events.setCanClimbDown();
};
