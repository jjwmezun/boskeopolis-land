#include "block.hpp"
#include "block_component_single_use.hpp"
#include "level.hpp"

BlockComponentSingleUse::BlockComponentSingleUse( int width, int height, int corner )
:
	width_ ( width ),
	height_ ( height ),
	corner_ ( corner )
{};

BlockComponentSingleUse::~BlockComponentSingleUse() {};

void BlockComponentSingleUse::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks )
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
