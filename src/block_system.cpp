#include "block_renderable.hpp"
#include "block_texture_renderable.hpp"
#include "block_system.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "level.hpp"
#include "level_state.hpp"
#include "render.hpp"
#include "map.hpp"
#include "sprite.hpp"
#include <utility>

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

static bool testBlockInTheWayWhetherCollided( const sdl2::SDLRect& rect, BlockComponent::Type type, const Block& b )
{
	return
		( type == BlockComponent::Type::NOTYPE || b.hasComponentTypeWhetherCollided( type ) ) &&
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

void BlockSystem::reset( LevelState& level_state )
{
	const auto& lvmap = level_state.currentMap();
	changeTileset( lvmap.tileset() );
	blocks_work_offscreen_ = lvmap.blocks_work_offscreen_;
	map_width_ = lvmap.widthBlocks();
	for ( const auto& layer : layers_ )
	{
		level_state.removeRenderable( layer.renderable_id_ );
	}
	layers_.clear();
};

void BlockSystem::renderLayer( const LevelState& level_state, int layer ) const
{	
	const Camera& camera = level_state.camera();
	if ( layer >= 0 && layer < layers_.size() )
	{
		for ( const auto& block : layers_[ layer ].blocks_ )
		{
			block.render( camera );
		}
	}
	/*
	if ( lvmap.hide_ )
	{
		return;
	}

	if ( !blocks_work_offscreen_ )
	{
		for ( const auto& blocks : layers_ )
		{
			for ( const auto& block : blocks )
			{
				block.render( camera );
			}
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
				for ( const auto& blocks : layers_ )
				{
					if ( n < blocks.size() )
					{
						const Block& block = blocks[ n ];
						block.render( camera );
					}
				}
			}
		}
	}*/
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
				for ( auto& layer : layers_ )
				{
					auto& blocks = layer.blocks_;
					if ( n < blocks.size() )
					{
						Block& block = blocks[ n ];
						block.interact( sprite, level_state );
					}
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
				for ( auto& layer : layers_ )
				{
					auto& blocks = layer.blocks_;
					if ( n < blocks.size() )
					{
						Block& block = blocks[ n ];
						block.interact( sprite, level_state );
					}
				}
			}
		}
	}
};

void BlockSystem::blocksFromMap( LevelState& level_state )
{
	Map& lvmap = level_state.currentMap();
	const Camera& camera = level_state.camera();
	const auto& map_blocks_layers = lvmap.blocksLayers();
	if ( !blocks_work_offscreen_ )
	{
		if ( camera.changed() || lvmap.changed_ )
		{
			if ( layers_.empty() )
			{
				// These need to be separate loops so the pointers aren’t moved between
				// setting level state renderables.
				for ( int li = 0; li < map_blocks_layers.size(); ++li )
				{
					layers_.push_back({ {}, level_state.addRenderable
					(
						(
							( map_blocks_layers[ li ].type_ == Map::LayerType::BLOCKS_TEXTURE )
								? std::unique_ptr<Renderable> ( new BlockTextureRenderable( li, level_state ) )
								: std::unique_ptr<Renderable> ( new BlockRenderable( li ) )
						),
						map_blocks_layers[ li ].layer_position_ )
					});
				}
			}
			else
			{
				for ( int li = 0; li < layers_.size(); ++li )
				{
					layers_[ li ].blocks_.clear();
				}
			}

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

					for ( int li = 0; li < map_blocks_layers.size(); ++li )
					{
						auto& blocks = map_blocks_layers[ li ].blocks_;
						const int type = ( i < blocks.size() ) ? blocks[ i ] - 1 : -1;
						addBlock( li, x_pixels, y_pixels, i, type, level_state );
					}
				}
			}
		}
	}
	else
	{
		if ( layers_.empty() )
		{
			// Create block object for every map tile, e'en if empty, so tile positions
			// can be used for optimizations in updating & rendering.
			for ( int li = 0; li < map_blocks_layers.size(); ++li )
			{
				layers_.push_back({ {}, level_state.addRenderable( std::unique_ptr<BlockRenderable> ( new BlockRenderable( li ) ), map_blocks_layers[ li ].layer_position_ ) } );

				const auto& blocks = map_blocks_layers[ li ].blocks_;
				for ( int i = 0; i < blocks.size(); ++i )
				{
					const int type = blocks[ i ] - 1;
					const int x = Unit::BlocksToPixels( lvmap.mapX( i ) );
					const int y = Unit::BlocksToPixels( lvmap.mapY( i ) );
					addBlock( li, x, y, i, type, level_state );
				}
			}
		}
	}
};

void BlockSystem::addBlock( int layer_index, int x, int y, int i, int type, LevelState& level_state )
{
	BlockType* block_type = getBlockType( type );
	auto& layer = layers_[ layer_index ].blocks_;
	layer.emplace_back( x, y, block_type, i, layer_index, type );
	layer[ layer.size() - 1 ].init( level_state );
};


void BlockSystem::renderLayerAllBlocks( const LevelState& level_state, int layer ) const
{
	const auto& lvmap = level_state.currentMap();
	const auto& blocks = lvmap.blocksLayers()[ layer ].blocks_;
	renderTiles( blocks, lvmap.widthBlocks(), lvmap.heightBlocks() );
};

void BlockSystem::renderTiles( const std::vector<int>& tiles, int width, int height ) const
{
	sdl2::SDLRect block_dest = { 0, 0, Unit::BlocksToPixels( 1 ), Unit::BlocksToPixels( 1 ) };
	sdl2::SDLRect tile_src = block_dest;
	for ( int y = 0; y < height; ++y )
	{
		for ( int x = 0; x < width; ++x )
		{
			const int i = mezun::nOfXY( x, y, width );
			const BlockType* type = getConstBlockType( tiles[ i ] - 1 );
			if ( type != nullptr && type->graphics() != nullptr && !type->graphics()->texture_.empty() )
			{
				const SpriteGraphics* graphics = type->graphics();
				block_dest.x = Unit::BlocksToPixels( x );
				block_dest.y = Unit::BlocksToPixels( y );
				tile_src.x = type->graphics()->current_frame_x_;
				tile_src.y = type->graphics()->current_frame_y_;
				Render::renderObject
				(
					graphics->texture_,
					tile_src,
					block_dest,
					graphics->flip_x_,
					graphics->flip_y_,
					graphics->rotation_,
					graphics->alpha_,
					nullptr
				);
			}
		}
	}
};

BlockType* BlockSystem::getBlockType( int type )
{
	return getTileset().blockType( type );
}

const BlockType* BlockSystem::getConstBlockType( int type ) const
{
	return getConstTileset().blockType( type );
}

bool BlockSystem::blocksInTheWay( const sdl2::SDLRect& rect, BlockComponent::Type type ) const
{
	return blocksInTheWayGeneric( rect, type, false );
}

bool BlockSystem::blocksInTheWayWhetherCollided( const sdl2::SDLRect& rect, BlockComponent::Type type ) const
{
	return blocksInTheWayGeneric( rect, type, true );
}

bool BlockSystem::blocksInTheWayGeneric( const sdl2::SDLRect& rect, BlockComponent::Type type, bool whether_collided ) const
{
	if ( !blocks_work_offscreen_ )
	{
		for ( const auto& layer : layers_ )
		{
			for ( const auto& block : layer.blocks_ )
			{
				if ( block.typeID() != -1 )
				{
					if ( ( whether_collided && testBlockInTheWayWhetherCollided( rect, type, block ) ) || ( !whether_collided && testBlockInTheWay( rect, type, block ) ) )
					{
						return true;
					}
				}
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
				for ( const auto& layer : layers_ )
				{
					const auto& blocks = layer.blocks_;
					if ( n < blocks.size() )
					{
						const Block& block = blocks[ n ];
						if ( block.typeID() != -1 )
						{
							if ( ( whether_collided && testBlockInTheWayWhetherCollided( rect, type, block ) ) || ( !whether_collided && testBlockInTheWay( rect, type, block ) ) )
							{
								return true;
							}
						}
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
		for ( const auto& layer : layers_ )
		{
			const auto& blocks = layer.blocks_;
			for ( const auto& block : blocks )
			{
				if ( block.typeID() != -1 && testBlockInTheWayExcept( rect, type, block ) )
				{
					return true;
				}
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
				for ( const auto& layer : layers_ )
				{
					const auto& blocks = layer.blocks_;
					if ( n < blocks.size() )
					{
						const Block& block = blocks[ n ];
						if ( block.typeID() != -1 && testBlockInTheWayExcept( rect, type, block ) )
						{
							return true;
						}
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

const Tileset& BlockSystem::getConstTileset() const
{
	auto t = tilesets_.find( current_tileset_ );
	assert( t != tilesets_.end() );
	return t->second;
};

// UNSAFE: FIX LATER
const std::vector<Block>& BlockSystem::getBlocksList() const
{
	return layers_[ 0 ].blocks_;
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
			for ( const auto& layer : layers_ )
			{
				const auto& blocks = layer.blocks_;
				if ( n < blocks.size() )
				{
					const Block& block = blocks[ n ];
					Collision collision = { 1, 1, 1, 1 };
					if ( block.testForComponentTypeNow( BlockComponent::Type::SOLID, collision, sprite, block, level_state ) )
					{
						block_sublist.push_back( &block );
					}
				}
			}
		}
	}
	return block_sublist;
}