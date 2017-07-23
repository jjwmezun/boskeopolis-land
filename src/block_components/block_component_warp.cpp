#include "block_component_warp.hpp"
#include "event_system.hpp"

BlockComponentWarp::BlockComponentWarp() {};

BlockComponentWarp::~BlockComponentWarp() {};

void BlockComponentWarp::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{
	events.changeMap();
};
