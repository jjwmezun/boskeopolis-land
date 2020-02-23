#include "block_system.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "level.hpp"
#include "level_state.hpp"
#include "render.hpp"
#include "map.hpp"
#include "sprite.hpp"
#include <utility>

#include <iostream>

static constexpr int CAMERA_PADDING = 8;
static constexpr int GRID_WIDTH = Unit::WINDOW_WIDTH_BLOCKS + CAMERA_PADDING * 2;
static constexpr int INTERACT_PADDING = 3 + CAMERA_PADDING;

static bool testBlockInTheWay( const sdl2::SDLRect& rect, BlockComponent::Type type, const Block& b )
{
	return
		( type == BlockComponent::Type::NOTYPE || b.hasComponentType( type ) ) &&
		b.rightSubPixels() > rect.x      &&
		b.leftSubPixels() < rect.right() &&
		b.topSubPixels() < rect.bottom() &&
		b.bottomSubPixels() > rect.y;
};
static bool testBlockInTheWayExcept( const sdl2::SDLRect& rect, BlockComponent::Type type, const Block& b )
{
	return
		( !b.hasComponentType( type ) ) &&
		b.rightSubPixels() > rect.x      &&
		b.leftSubPixels() < rect.right() &&
		b.topSubPixels() < rect.bottom() &&
		b.bottomSubPixels() > rect.y;
};

void BlockSystem::init( const Map& lvmap )
{
	blocks_work_offscreen_ = lvmap.blocks_work_offscreen_;
	map_width_ = lvmap.widthBlocks();
	tilesets_.insert( std::make_pair( lvmap.tileset(), ( lvmap.tileset() ) ) );
	current_tileset_ = lvmap.tileset();
}

void BlockSystem::update( EventSystem& events )
{
	getTileset().update( events );
};

void BlockSystem::reset( const Map& lvmap )
{
	changeTileset( lvmap.tileset() );
	blocks_work_offscreen_ = lvmap.blocks_work_offscreen_;
	map_width_ = lvmap.widthBlocks();
};

void BlockSystem::render( const Map& lvmap, const Camera& camera, bool priority )
{
	if ( lvmap.hide_ )
	{
		return;
	}

	if ( !blocks_work_offscreen_ )
	{
		for ( const auto& block : blocks_ )
		{
			block.render( camera, priority );
		}
	}
	else
	{
		// Only render blocks in-camera.
		const int first_x = Unit::PixelsToBlocks( camera.x() ) - 2; // Block x just left o' camera.
		const int first_y = Unit::PixelsToBlocks( camera.y() ) - 2; // Block y just 'bove camera.
		const int last_x  = Unit::PixelsToBlocks( camera.screenRight() ) + 2; // Block x just right o' screen.
		const int last_y  = Unit::PixelsToBlocks( camera.screenBottom() ) + 2; // Block y just below screen.
		for ( int y = first_y; y < last_y; ++y )
		{
			for ( int x = first_x; x < last_x; ++x )
			{
				int n = y * lvmap.widthBlocks() + x;
				if ( n < blocks_.size() )
				{
					const Block& block = blocks_[ n ];
					block.render( camera, priority );
				}
			}
		}
	}
};

void BlockSystem::interact( Sprite& sprite, LevelState& level_state )
{
	const Camera& camera = level_state.camera();
	if ( !blocks_work_offscreen_ )
	{
		// Only test sprite interaction with blocks round it.
		const int first_x = floor( camera.relativeX( sprite.xPixels()      ) / Unit::PIXELS_PER_BLOCK ) - INTERACT_PADDING; // Block x a bit left o' sprite.
		const int first_y = floor( camera.relativeY( sprite.yPixels()      ) / Unit::PIXELS_PER_BLOCK ) - INTERACT_PADDING; // Block y a bit 'bove sprite.
		const int last_x  = ceil( camera.relativeX( sprite.rightPixels()  ) / Unit::PIXELS_PER_BLOCK ) + INTERACT_PADDING; // Block x a bit right o' sprite.
		const int last_y  = ceil( camera.relativeY( sprite.bottomPixels() ) / Unit::PIXELS_PER_BLOCK ) + INTERACT_PADDING; // Block y a bit below sprite.
		for ( int y = first_y; y < last_y; ++y )
		{
			for ( int x = first_x; x < last_x; ++x )
			{
				const int n = y * GRID_WIDTH + x;
				if ( n < blocks_.size() )
				{
					Block& block = blocks_[ n ];
					block.interact( sprite, level_state );
				}
			}
		}
	}
	else
	{
		// Only test sprite interaction with blocks round it.
		const int first_x = floor( sprite.xPixels()      / Unit::PIXELS_PER_BLOCK ) - 3; // Block x a bit left o' sprite.
		const int first_y = floor( sprite.yPixels()      / Unit::PIXELS_PER_BLOCK ) - 3; // Block y a bit 'bove sprite.
		const int last_x  = ceil ( sprite.rightPixels()  / Unit::PIXELS_PER_BLOCK ) + 3; // Block x a bit right o' sprite.
		const int last_y  = ceil ( sprite.bottomPixels() / Unit::PIXELS_PER_BLOCK ) + 3; // Block y a bit below sprite.
		for ( int y = first_y; y < last_y; ++y )
		{
			for ( int x = first_x; x < last_x; ++x )
			{
				const int n = y * level_state.currentMap().widthBlocks() + x;
				if ( n < blocks_.size() )
				{
					Block& block = blocks_[ n ];
					block.interact( sprite, level_state );
				}
			}
		}
	}
};

void BlockSystem::blocksFromMap( LevelState& level_state )
{
	Map& lvmap = level_state.currentMap();
	const Camera& camera = level_state.camera();
	if ( !blocks_work_offscreen_ )
	{
		if ( camera.changed() || lvmap.changed_ )
		{
			blocks_.clear();
			const int first_x = Unit::PixelsToBlocks( camera.x() ) - CAMERA_PADDING; // Block a bit left o' camera.
			const int first_y = Unit::PixelsToBlocks( camera.y() ) - CAMERA_PADDING; // Block a bit 'bove camera.
			const int last_x  = Unit::PixelsToBlocks( camera.screenRight() ) + CAMERA_PADDING; // Block a bit right o' camera.
			const int last_y  = Unit::PixelsToBlocks( camera.screenBottom() ) + CAMERA_PADDING; // Block a bit below camera.

			for ( int y = first_y; y < last_y; ++y )
			{
				const int y_pixels = Unit::BlocksToPixels( y );
				for ( int x = first_x; x < last_x; ++x )
				{
					const int x_pixels = Unit::BlocksToPixels( x );
					const int i = lvmap.indexFromXAndY( x, y );
					const int type = lvmap.block( i ) - 1;
					addBlock( x_pixels, y_pixels, i, type );
				}
			}

		}
	}
	else
	{
		if ( blocks_.empty() )
		{
			// Create block object for every map tile, e'en if empty, so tile positions
			// can be used for optimizations in updating & rendering.
			for ( int i = 0; i < lvmap.blocksSize(); ++i )
			{
				const int type = lvmap.block( i ) - 1;
				const int x = Unit::BlocksToPixels( lvmap.mapX( i ) );
				const int y = Unit::BlocksToPixels( lvmap.mapY( i ) );
				addBlock( x, y, i, type );
				blocks_[ i ].init( level_state );
			}
		}
	}
};

void BlockSystem::addBlock( int x, int y, int i, int type )
{
	BlockType* block_type = getBlockType( type );
	blocks_.emplace_back( x, y, block_type, i, type );
};

BlockType* BlockSystem::getBlockType( int type )
{
	return getTileset().blockType( type );
}

bool BlockSystem::blocksInTheWay( const sdl2::SDLRect& rect, BlockComponent::Type type ) const
{
	if ( !blocks_work_offscreen_ )
	{
		for ( const auto& block : blocks_ )
		{
			if ( block.typeID() != -1 && testBlockInTheWay( rect, type, block ) )
			{
				return true;
			}
		}
	}
	else
	{
		// Only test blocks near the rect.
		const int first_x = floor( rect.x        / Unit::SUBPIXELS_PER_BLOCK ) - 3; // Block x a bit left o' rect.
		const int first_y = floor( rect.y        / Unit::SUBPIXELS_PER_BLOCK ) - 3; // Block y a bit 'bove rect.
		const int last_x  = ceil ( rect.right()  / Unit::SUBPIXELS_PER_BLOCK ) + 3; // Block x a bit right o' rect.
		const int last_y  = ceil ( rect.bottom() / Unit::SUBPIXELS_PER_BLOCK ) + 3; // Block y a bit below rect.
		for ( int y = first_y; y < last_y; ++y )
		{
			for ( int x = first_x; x < last_x; ++x )
			{
				const int n = y * map_width_ + x;
				if ( n < blocks_.size() )
				{
					const Block& block = blocks_[ n ];
					if ( block.typeID() != -1 && testBlockInTheWay( rect, type, block ) )
					{
						return true;
					}
				}
			}
		}
	}
	return false;
};

bool BlockSystem::blocksInTheWayExcept( const sdl2::SDLRect& rect, BlockComponent::Type type ) const
{
	if ( !blocks_work_offscreen_ )
	{
		for ( const auto& block : blocks_ )
		{
			if ( block.typeID() != -1 && testBlockInTheWayExcept( rect, type, block ) )
			{
				return true;
			}
		}
	}
	else
	{
		// Only test blocks near the rect.
		const int first_x = floor( rect.x        / Unit::SUBPIXELS_PER_BLOCK ) - 3; // Block x a bit left o' rect.
		const int first_y = floor( rect.y        / Unit::SUBPIXELS_PER_BLOCK ) - 3; // Block y a bit 'bove rect.
		const int last_x  = ceil ( rect.right()  / Unit::SUBPIXELS_PER_BLOCK ) + 3; // Block x a bit right o' rect.
		const int last_y  = ceil ( rect.bottom() / Unit::SUBPIXELS_PER_BLOCK ) + 3; // Block y a bit below rect.
		for ( int y = first_y; y < last_y; ++y )
		{
			for ( int x = first_x; x < last_x; ++x )
			{
				const int n = y * map_width_ + x;
				if ( n < blocks_.size() )
				{
					const Block& block = blocks_[ n ];
					if ( block.typeID() != -1 && testBlockInTheWayExcept( rect, type, block ) )
					{
						return true;
					}
				}
			}
		}
	}
	return false;
};

void BlockSystem::changeTileset( std::string new_tileset )
{
	// See if tileset already exists.
	auto i = tilesets_.find( new_tileset );

	// If Tileset doesn't already exist, try making 1.
	if ( i == tilesets_.end() )
	{
		tilesets_.insert( std::make_pair( new_tileset, ( new_tileset ) ) );
	}
	current_tileset_ = new_tileset;
};

Tileset& BlockSystem::getTileset()
{
	auto t = tilesets_.find( current_tileset_ );
	assert( t != tilesets_.end() );
	return t->second;
};

const std::vector<Block>& BlockSystem::getBlocksList() const
{
	return blocks_;
}

const std::vector<const Block*> BlockSystem::getSolidBlocksInField( const sdl2::SDLRect& rect, const Camera& camera, const Sprite& sprite, LevelState& level_state ) const
{
	std::vector<const Block*> block_sublist = {};
	const sdl2::SDLRect relative_box = camera.relativeRect( Unit::SubPixelsToPixels( rect ) );
	// Only test with blocks round it.
	const int first_x = Unit::PixelsToBlocks( relative_box.x ) + 6; // Block x a bit left o' box
	const int first_y = Unit::PixelsToBlocks( relative_box.y ) + 7; // Block y a bit 'bove box.
	const int last_x  = ( int )( ceil ( ( double )( relative_box.right() ) / ( double )( Unit::PIXELS_PER_BLOCK ) ) ) + 10; // Block x a bit right o' box.
	const int last_y  = ( int )( ceil ( ( double )( relative_box.bottom() ) / ( double )( Unit::PIXELS_PER_BLOCK ) ) ) + 11; // Block y a bit below box.

	for ( int y = first_y; y < last_y; ++y )
	{
		for ( int x = first_x; x < last_x; ++x )
		{
			const int n = y * GRID_WIDTH + x;
			if ( n < blocks_.size() )
			{
				const Block& block = blocks_[ n ];
				Collision collision = { 1, 1, 1, 1 };
				if ( block.testForComponentTypeNow( BlockComponent::Type::SOLID, collision, sprite, block, level_state ) )
				{
					block_sublist.push_back( &block );
				}
			}
		}
	}
	return block_sublist;
}