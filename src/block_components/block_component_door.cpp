#include "block_component_door.hpp"
#include "event_system.hpp"

BlockComponentDoor::BlockComponentDoor() {};
BlockComponentDoor::~BlockComponentDoor() {};

void BlockComponentDoor::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health )
{
	events.in_front_of_door_ = true;
};