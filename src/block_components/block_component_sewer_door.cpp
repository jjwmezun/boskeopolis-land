#include "block_component_sewer_door.hpp"
#include "event_system.hpp"

BlockComponentSewerDoor::BlockComponentSewerDoor() {};
BlockComponentSewerDoor::~BlockComponentSewerDoor() {};

void BlockComponentSewerDoor::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites )
{
	events.in_front_of_door_ = 2;
};
