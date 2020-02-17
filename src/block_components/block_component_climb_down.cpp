#include "block_component_climb_down.hpp"
#include "event_system.hpp"

BlockComponentClimbDown::BlockComponentClimbDown() {};

BlockComponentClimbDown::~BlockComponentClimbDown() {};

void BlockComponentClimbDown::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	events.setCanClimbDown();
};
