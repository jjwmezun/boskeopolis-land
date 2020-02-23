#include "block_component_heal.hpp"
#include "health.hpp"

BlockComponentHeal::BlockComponentHeal() {};

BlockComponentHeal::~BlockComponentHeal() {};

void BlockComponentHeal::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	health.heal();
};
