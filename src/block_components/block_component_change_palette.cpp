#include "block_component_change_palette.hpp"
#include "event_system.hpp"
#include "level_state.hpp"
#include "sprite.hpp"

BlockComponentChangePalette::BlockComponentChangePalette( const Palette& palette ) : palette_ ( palette ) {};

BlockComponentChangePalette::~BlockComponentChangePalette() {};

void BlockComponentChangePalette::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	level_state.events().changePalette( palette_ );
};
