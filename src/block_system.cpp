#include "block_system.hpp"
#include "camera.hpp"
#include "mezun_exceptions.hpp"
#include "mezun_helpers.hpp"
#include "render.hpp"
#include <iostream>
#include "level.hpp"
#include "sprite.hpp"
#include <utility>

BlockSystem::BlockSystem( const Level& level )
{	
	tilesets_.emplace_back( Tileset{ 0 } );

	for ( auto& m : level.allMaps() )
	{
		const int tileset = m.tileset();
		if ( mezun::notInMap( tileset_ids_, tileset ) )
		{
			tilesets_.emplace_back( Tileset{ tileset } );
			tileset_ids_.insert( std::make_pair( tileset, lastTilesetIndex() ) );
		}
	}
	setCurrentTileset( level.currentMap() );
};

void BlockSystem::update( EventSystem& events )
{
	universalTileset().update( events );
	mapTileset().update( events );
};

void BlockSystem::render( const Camera& camera, bool priority )
{
	for ( auto& b : blocks_ )
	{
		if ( camera.onscreen( b.hitBox() ) )
		{
			b.render( camera, priority );
		}
	}
};

void BlockSystem::interact( Sprite& sprite, Level& level, EventSystem& events, Camera& camera, Health& health )
{
	for ( auto& b : blocks_ )
	{
		b.interact( sprite, level, events, camera, health );
	}
};

void BlockSystem::blocksFromMap( const Map& lvmap, const Camera& camera )
{
	if ( !lvmap.blocksWorkOffscreen() )
	{
		if ( camera.changed() || lvmap.changed() )
		{
			blocks_.clear();

			const int first_x = floor( camera.x() / Unit::PIXELS_PER_BLOCK ) - CAMERA_PADDING;
			const int first_y = floor( camera.y() / Unit::PIXELS_PER_BLOCK ) - CAMERA_PADDING;
			const int last_x = ceil( camera.right() / Unit::PIXELS_PER_BLOCK ) + CAMERA_PADDING;
			const int last_y = ceil( camera.bottom() / Unit::PIXELS_PER_BLOCK ) + CAMERA_PADDING;

			for ( int y = first_y; y < last_y; ++y )
			{
				const int yp = Unit::BlocksToPixels( y );

				for ( int x = first_x; x < last_x; ++x )
				{
					const int xp = Unit::BlocksToPixels( x );

					const int i = lvmap.indexFromXAndY( x, y );
					const int type = lvmap.block( i ) - 1;

					addBlock( xp, yp, i, type, blocks_ );
				}
			}

		}
	}
	else
	{
		if ( blocks_.empty() )
		{
			for ( int i = 0; i < lvmap.blocksSize(); ++i )
			{
				const int type = lvmap.block( i ) - 1;
				
				if ( type > -1 )
				{
					const int x = Unit::BlocksToPixels( lvmap.mapX( i ) );
					const int y = Unit::BlocksToPixels( lvmap.mapY( i ) );

					addBlock( x, y, i, type, blocks_ );

				}
			}
		}
		else
		{
			for ( int i = 0; i < blocks_.size(); ++i )
			{
				if ( blocks_.at( i ).destroyed_ ) blocks_.erase( blocks_.begin() + i );
			}
		}
	}
};

void BlockSystem::addBlock( int x, int y, int i, int type, std::vector<Block>& list )
{
	BlockType* block_type;

	if ( type <= Tileset::EMPTY_BLOCK )
	{
		return;
	}
	else if ( type < Tileset::UNIVERSAL_TILESET_SIZE )
	{
		block_type = universalTileset().blockType( type, x, y );
	}
	else
	{
		block_type = mapTileset().blockType( type - Tileset::UNIVERSAL_TILESET_SIZE, x, y );
	}

	if ( block_type != nullptr )
	{
		list.emplace_back( x, y, block_type, i, type );
	}
};

bool BlockSystem::blocksInTheWay( const sdl::rect& r, BlockComponent::Type type ) const
{
	for ( auto& b : blocks_ )
	{
		if
		(
			b.rightSubPixels() > r.x &&
			b.leftSubPixels() < r.right() &&
			b.topSubPixels() < r.bottom() &&
			b.bottomSubPixels() > r.y
		)
		{
			if ( b.hasComponentType( type ) )
			{
				return true;
			}
		}
	}

	return false;
};

void BlockSystem::changedMap( const Map& lvmap )
{
	setCurrentTileset( lvmap );
};

Tileset& BlockSystem::universalTileset()
{
	return tilesets_[ 0 ];
};

Tileset& BlockSystem::mapTileset()
{
	return tilesets_[ current_tileset_ ];
};

void BlockSystem::setCurrentTileset( const Map& lvmap )
{
	current_tileset_ = mezun::findInMap( tileset_ids_, lvmap.tileset() );
};

int BlockSystem::lastTilesetIndex() const
{
	return tilesets_.size() - 1;
};