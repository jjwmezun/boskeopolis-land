#include "block.hpp"
#include "block_component_sewer_door.hpp"
#include "event_system.hpp"

BlockComponentSewerDoor::BlockComponentSewerDoor() {};
BlockComponentSewerDoor::~BlockComponentSewerDoor() {};

void BlockComponentSewerDoor::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	events.setInFrontOfSewerDoor();
	events.setSewerPosition( block.hit_box_.x - 16000, block.hit_box_.y - 32000 );
};
