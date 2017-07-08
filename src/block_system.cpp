
// Name
//===================================
//
// BlockSystem
//


// DEPENDENCIES
//===================================

	#include "block_system.h"
    #include "camera.h"
	#include "mezun_exceptions.h"
    #include "graphics.h"
    #include <iostream>
    #include "map.h"
    #include "sprite.h"
    #include <utility>


// STATIC VARIABLES
//===================================


// METHODS
//===================================

	BlockSystem::BlockSystem( const Map& lvmap )
	{
		try
		{
			tilesets_.insert( std::make_pair( lvmap.tileset(), ( lvmap.tileset() ) ) );
			current_tileset_ = lvmap.tileset();
		}
		catch ( const mezun::CantLoadTileset& e )
		{
			throw e;
		}
	};

    void BlockSystem::update( EventSystem& events )
    {
		getTileset().update( events );
    };

    void BlockSystem::render( Graphics& graphics, Camera& camera, bool priority )
    {
        for ( auto& b : blocks_ )
        {
			if ( camera.onscreen( b.hitBox() ) )
			{
            	b.render( graphics, camera, priority );
			}
        }
    };

    void BlockSystem::interact( Sprite& sprite, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
    {
        for ( auto& b : blocks_ )
        {
            b.interact( sprite, level, events, inventory, camera );
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

						addBlock( xp, yp, i, type );
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

						addBlock( x, y, i, type );

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

	void BlockSystem::addBlock( int x, int y, int i, int type )
	{
		BlockType* block_type = getTileset().blockType( type, x, y );

		if ( block_type != nullptr )
		{
			blocks_.emplace_back( x, y, block_type, i, type );
		}
	}

    bool BlockSystem::blocksInTheWay( const sdl2::SDLRect& r, BlockComponent::Type type ) const
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

	Tileset& BlockSystem::getTileset()
	{
		auto t = tilesets_.find( current_tileset_ );
		
		if ( t == tilesets_.end() )
		{
			throw mezun::MissingTileset( current_tileset_ );
		}
		
		return t->second;
	};