#include <algorithm>
#include <iostream>
#include "block_system.h"
#include "camera.h"
#include "map_layer_tilemap.h"
#include "mezun_math.h"

MapLayerTilemap::MapLayerTilemap( const std::vector<int>& tiles, int map_width, int map_height )
:
	MapLayer(),
	tiles_ ( tiles ),
	width_ ( map_width ),
	height_ ( map_height ),
	blocks_ ()
{
};

MapLayerTilemap::~MapLayerTilemap() {};

void MapLayerTilemap::update( EventSystem& events, BlockSystem& blocks, const Camera& camera )
{
	if ( camera.changed() )
	{
		blocks_.clear();

		const int first_x = std::max( ( int )( floor( camera.x() / Unit::PIXELS_PER_BLOCK ) - CAMERA_PADDING ), 0 );
		const int first_y = std::max( ( int )( floor( camera.y() / Unit::PIXELS_PER_BLOCK ) - CAMERA_PADDING ), 0 );
		const int last_x = std::min( ( int )( ceil( camera.right() / Unit::PIXELS_PER_BLOCK ) + CAMERA_PADDING ), width_ - 1 );
		const int last_y = std::min( ( int )( ceil( camera.bottom() / Unit::PIXELS_PER_BLOCK ) + CAMERA_PADDING ), height_ - 1 );

		for ( int y = first_y; y < last_y; ++y )
		{
			const int yp = Unit::BlocksToPixels( y );

			for ( int x = first_x; x < last_x; ++x )
			{
				const int xp = Unit::BlocksToPixels( x );

				const int i = nOfXY( x, y );
				const int type = tiles_.at( i ) - 1;

				blocks.addBlock( xp, yp, i, type, blocks_ );
			}
		}
	}
};

void MapLayerTilemap::render( Graphics& graphics, Camera& camera ) const
{
	for ( auto& b : blocks_ )
	{
		if ( camera.onscreen( b.hitBox() ) )
		{
			b.render( graphics, camera, false );
		}
	}
};

int MapLayerTilemap::nOfXY( int x, int y ) const
{
	return mezun::nOfXY( x, y, width_ );
};