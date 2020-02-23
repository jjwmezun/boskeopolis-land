#include "block_component_full_heal.hpp"
#include "health.hpp"

BlockComponentFullHeal::BlockComponentFullHeal() {};

BlockComponentFullHeal::~BlockComponentFullHeal() {};

void BlockComponentFullHeal::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	health.fullHeal();
};