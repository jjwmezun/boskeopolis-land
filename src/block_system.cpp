#include "block_system.hpp"
#include "camera.hpp"
#include "level.hpp"
#include "render.hpp"
#include "map.hpp"
#include "sprite.hpp"
#include <utility>

static constexpr int CAMERA_PADDING = 8;
static bool testBlockInTheWay( const sdl2::SDLRect& rect, BlockComponent::Type type, const Block& b )
{
	return
		b.hasComponentType( type )       &&
		b.rightSubPixels() > rect.x      &&
		b.leftSubPixels() < rect.right() &&
		b.topSubPixels() < rect.bottom() &&
		b.bottomSubPixels() > rect.y;
};

BlockSystem::BlockSystem( const Map& lvmap )
:
	blocks_work_offscreen_ ( lvmap.blocks_work_offscreen_ ),
	map_width_ ( lvmap.widthBlocks() )
{
	tilesets_.insert( std::make_pair( lvmap.tileset(), ( lvmap.tileset() ) ) );
	current_tileset_ = lvmap.tileset();
};

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
		const int first_x = floor( camera.x()      / Unit::PIXELS_PER_BLOCK ) - 2; // Block x just left o' camera.
		const int first_y = floor( camera.y()      / Unit::PIXELS_PER_BLOCK ) - 2; // Block y just 'bove camera.
		const int last_x  = ceil ( camera.right()  / Unit::PIXELS_PER_BLOCK ) + 2; // Block x just right o' camera.
		const int last_y  = ceil ( camera.bottom() / Unit::PIXELS_PER_BLOCK ) + 2; // Block y just below camera.
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

void BlockSystem::interact( Sprite& sprite, Level& level, EventSystem& events, Camera& camera, Health& health, SpriteSystem& sprites )
{
	if ( !blocks_work_offscreen_ )
	{
		for ( Block& block : blocks_ )
		{
			block.interact( sprite, level, events, camera, health, *this, sprites );
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
				const int n = y * level.currentMap().widthBlocks() + x;
				if ( n < blocks_.size() )
				{
					Block& block = blocks_[ n ];
					block.interact( sprite, level, events, camera, health, *this, sprites );
				}
			}
		}
	}
};

void BlockSystem::blocksFromMap( Map& lvmap, const Camera& camera )
{
	if ( !blocks_work_offscreen_ )
	{
		if ( camera.changed() || lvmap.changed_ )
		{
			blocks_.clear();

			const int first_x = floor( camera.x()      / Unit::PIXELS_PER_BLOCK ) - CAMERA_PADDING; // Block a bit left o' camera.
			const int first_y = floor( camera.y()      / Unit::PIXELS_PER_BLOCK ) - CAMERA_PADDING; // Block a bit 'bove camera.
			const int last_x  = ceil ( camera.right()  / Unit::PIXELS_PER_BLOCK ) + CAMERA_PADDING; // Block a bit right o' camera.
			const int last_y  = ceil ( camera.bottom() / Unit::PIXELS_PER_BLOCK ) + CAMERA_PADDING; // Block a bit below camera.

			for ( int y = first_y; y < last_y; ++y )
			{
				const int y_pixels = Unit::BlocksToPixels( y );
				for ( int x = first_x; x < last_x; ++x )
				{
					const int x_pixels = Unit::BlocksToPixels( x );
					const int i = lvmap.indexFromXAndY( x, y );
					const int type = lvmap.block( i ) - 1;
					addBlock( x_pixels, y_pixels, i, type, false );
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
				addBlock( x, y, i, type, true );
				blocks_[ i ].init( lvmap );
			}
		}
	}
};

void BlockSystem::addBlock( int x, int y, int i, int type, bool accept_all_blocks )
{
	BlockType* block_type = getBlockType( type );
	if ( accept_all_blocks || block_type != nullptr )
	{
		blocks_.emplace_back( x, y, block_type, i, type );
	}
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
			if ( testBlockInTheWay( rect, type, block ) )
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
					if ( testBlockInTheWay( rect, type, block ) )
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
