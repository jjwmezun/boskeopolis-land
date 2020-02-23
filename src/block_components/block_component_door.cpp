#include "block_component_door.hpp"
#include "event_system.hpp"

BlockComponentDoor::BlockComponentDoor() {};
BlockComponentDoor::~BlockComponentDoor() {};

void BlockComponentDoor::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	events.setInFrontOfDoor();
};
