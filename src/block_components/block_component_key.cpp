#include "block_component_key.hpp"
#include "event_system.hpp"

BlockComponentKey::BlockComponentKey() {};

BlockComponentKey::~BlockComponentKey() {};

void BlockComponentKey::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{
	events.getKey();
};
