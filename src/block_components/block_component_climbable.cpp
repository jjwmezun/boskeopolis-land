#include "block_component_climbable.hpp"
#include "event_system.hpp"

BlockComponentClimbable::BlockComponentClimbable() {};

BlockComponentClimbable::~BlockComponentClimbable() {};

void BlockComponentClimbable::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	events.setTouchingLadder();
};
