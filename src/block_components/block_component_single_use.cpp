#include "block.hpp"
#include "block_component_single_use.hpp"
#include "level.hpp"
#include "level_state.hpp"

BlockComponentSingleUse::BlockComponentSingleUse( int width, int height, int corner )
:
	width_ ( width ),
	height_ ( height ),
	corner_ ( corner )
{};

BlockComponentSingleUse::~BlockComponentSingleUse() {};

void BlockComponentSingleUse::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	for ( int y = 0; y < height_; ++y )
	{
		const int cy = floor( corner_ / width_ );
		const int ly = ( level_state.level().currentMap().mapY( block.location() ) - cy + y ) * level_state.level().currentMap().widthBlocks();

		for ( int x = 0; x < width_; ++x )
		{
			const int cx = corner_ % width_;
			const int lx = ( level_state.level().currentMap().mapX( block.location() ) - cx + x );

			const int location = ly + lx;

			level_state.level().currentMap().deleteBlock( block.layer(), location );
			block.destroy();
		}
	}
};
