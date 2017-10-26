#include "block_component_lose.hpp"
#include "event_system.hpp"

BlockComponentLose::BlockComponentLose() {};

BlockComponentLose::~BlockComponentLose() {};

void BlockComponentLose::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks )
{
	events.fail();
};
