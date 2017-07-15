#include "block_component_door.h"
#include "event_system.h"

BlockComponentDoor::BlockComponentDoor() {};
BlockComponentDoor::~BlockComponentDoor() {};

void BlockComponentDoor::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{
	events.in_front_of_door_ = true;
};