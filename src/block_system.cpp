#include "block_system.hpp"
#include "camera.hpp"
#include "mezun_exceptions.hpp"
#include "render.hpp"
#include <iostream>
#include "map.hpp"
#include "sprite.hpp"
#include <utility>

BlockSystem::BlockSystem( const Map& lvmap )
{
	tilesets_.insert( std::make_pair( "universal", ( "universal" ) ) );
	tilesets_.insert( std::make_pair( lvmap.tileset(), ( lvmap.tileset() ) ) );
	current_tileset_ = lvmap.tileset();
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

Tileset& BlockSystem::universalTileset()
{
	auto t = tilesets_.find( "universal" );

	if ( t == tilesets_.end() )
	{
		mezun::error( "Universal tileset ne'er loaded in BlockSystem." );
	}

	return t->second;	
};

Tileset& BlockSystem::mapTileset()
{
	auto t = tilesets_.find( current_tileset_ );

	if ( t == tilesets_.end() )
	{
		mezun::error( "Map tileset \"" + current_tileset_ + "\" ne'er loaded in BlockSystem." );
	}

	return t->second;
};