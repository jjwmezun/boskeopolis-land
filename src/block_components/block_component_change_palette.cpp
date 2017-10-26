#include "block_component_change_palette.hpp"
#include "event_system.hpp"
#include "sprite.hpp"

BlockComponentChangePalette::BlockComponentChangePalette( const Palette& palette ) : palette_ ( palette ) {};

BlockComponentChangePalette::~BlockComponentChangePalette() {};

void BlockComponentChangePalette::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks )
{
	events.changePalette( palette_ );
};
