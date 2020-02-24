#include "audio.hpp"
#include "block_component_mcguffin.hpp"
#include "inventory_level.hpp"
#include "level_state.hpp"

BlockComponentMcGuffin::BlockComponentMcGuffin() {};
BlockComponentMcGuffin::~BlockComponentMcGuffin() {};

void BlockComponentMcGuffin::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	level_state.inventory().addMcGuffin();
	Audio::playSound( Audio::SoundType::ITEM );
};