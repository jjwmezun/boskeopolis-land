#include "block.h"
#include "block_component_single_use.h"
#include "level.h"

BlockComponentSingleUse::BlockComponentSingleUse( int width, int height, int corner )
:
	width_ ( width ),
	height_ ( height ),
	corner_ ( corner )
{};

BlockComponentSingleUse::~BlockComponentSingleUse() {};

void BlockComponentSingleUse::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{
	for ( int y = 0; y < height_; ++y )
	{
		int cy = floor( corner_ / width_ );
		int ly = ( level.currentMap().mapY( block.location() ) - cy + y ) * level.currentMap().widthBlocks();

		for ( int x = 0; x < width_; ++x )
		{
			int cx = corner_ % width_;
			int lx = ( level.currentMap().mapX( block.location() ) - cx + x );

			int location = ly + lx;

			level.currentMap().deleteBlock( location );
			block.destroy();
		}
	}
};
