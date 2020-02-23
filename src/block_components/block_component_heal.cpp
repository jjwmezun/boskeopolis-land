#include "block_component_heal.hpp"
#include "health.hpp"
#include "level_state.hpp"

BlockComponentHeal::BlockComponentHeal() {};

BlockComponentHeal::~BlockComponentHeal() {};

void BlockComponentHeal::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	level_state.health().heal();
};
