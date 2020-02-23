#include "block_component_warp.hpp"
#include "event_system.hpp"
#include "level_state.hpp"

BlockComponentWarp::BlockComponentWarp() {};

BlockComponentWarp::~BlockComponentWarp() {};

void BlockComponentWarp::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	level_state.events().changeMap();
};
