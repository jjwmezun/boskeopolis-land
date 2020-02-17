#include "block_component_door.hpp"
#include "event_system.hpp"

BlockComponentDoor::BlockComponentDoor() {};
BlockComponentDoor::~BlockComponentDoor() {};

void BlockComponentDoor::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	events.setInFrontOfDoor();
};
