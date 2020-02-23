#include "block_component_lose.hpp"
#include "event_system.hpp"
#include "level_state.hpp"

BlockComponentLose::BlockComponentLose() {};

BlockComponentLose::~BlockComponentLose() {};

void BlockComponentLose::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	level_state.events().fail();
};
