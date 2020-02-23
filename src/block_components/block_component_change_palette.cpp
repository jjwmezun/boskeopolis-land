#include "block_component_change_palette.hpp"
#include "event_system.hpp"
#include "sprite.hpp"

BlockComponentChangePalette::BlockComponentChangePalette( const Palette& palette ) : palette_ ( palette ) {};

BlockComponentChangePalette::~BlockComponentChangePalette() {};

void BlockComponentChangePalette::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	events.changePalette( palette_ );
};
