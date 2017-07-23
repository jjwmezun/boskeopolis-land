#include "block_component_warp.hpp"
#include "event_system.hpp"

BlockComponentWarp::BlockComponentWarp() {};

BlockComponentWarp::~BlockComponentWarp() {};

void BlockComponentWarp::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera )
{
	events.changeMap();
};
