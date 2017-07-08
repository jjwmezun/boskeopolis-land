#include "block_component_warp.h"
#include "event_system.h"

BlockComponentWarp::BlockComponentWarp() {};

BlockComponentWarp::~BlockComponentWarp() {};

void BlockComponentWarp::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{
	events.changeMap();
};
