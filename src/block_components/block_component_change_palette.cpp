#include "block_component_change_palette.h"
#include "event_system.h"
#include "sprite.h"

BlockComponentChangePalette::BlockComponentChangePalette( const Palette& palette ) : palette_ ( palette ) {};

BlockComponentChangePalette::~BlockComponentChangePalette() {};

void BlockComponentChangePalette::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{
	events.changePalette( palette_ );
};
