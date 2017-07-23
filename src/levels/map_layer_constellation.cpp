#include "camera.h"
#include <cassert>
#include "game.h"
#include <iostream>
#include "map_layer_constellation.h"
#include "render.h"

MapLayerConstellation::MapLayerConstellation
(
	int map_width_blocks,
	int map_height_blocks,
	double scroll_speed_x,
	double scroll_speed_y
)
:
	MapLayer(),
	texture_ ( "bg" + Game::pathDivider() + "constellation.png" ),
	scroll_speed_x_ ( scroll_speed_x ),
	scroll_speed_y_ ( scroll_speed_y ),
	height_ ( calculateWidth( map_height_blocks ) ),
	width_ ( calculateWidth( map_width_blocks ) ),
	star_pattern_ ( generateStarPattern() )
{};

MapLayerConstellation::~MapLayerConstellation() {};

void MapLayerConstellation::render( Camera& camera ) const
{
	sdl2::SDLRect source = { 0, 0, TILE_SIZE, TILE_SIZE };
	sdl2::SDLRect dest = { 0, 0, TILE_SIZE, TILE_SIZE };

	const int first_x = ( floor( camera.x() / TILE_SIZE ) * scroll_speed_x_ );
	const int first_y = ( floor( camera.y() / TILE_SIZE ) * scroll_speed_y_ );
	const int last_x = Unit::PixelsToMiniBlocks( camera.widthPixels() ) + first_x + 1;
	const int last_y = Unit::PixelsToMiniBlocks( camera.heightPixels() ) + first_y + 1;

	dest.y = -( (int)( camera.y() * scroll_speed_y_ ) % TILE_SIZE );

	for ( int y = first_y; y < last_y; ++y )
	{
		dest.x = -( (int)( camera.x() * scroll_speed_x_ ) % TILE_SIZE );

		for ( int x = first_x; x < last_x; ++x )
		{
			assert( indexFromXAndY( x, y ) < star_pattern_.size() );

			if ( star_pattern_.at( indexFromXAndY( x, y ) ) != StarType::NO_STAR )
			{
				source.x = ( int )( star_pattern_.at( indexFromXAndY( x, y ) ) ) * TILE_SIZE;

				Render::renderObject( texture_, source, dest );
			}

			dest.x += TILE_SIZE;
		}

		dest.y += TILE_SIZE;
	}
};

const std::vector<MapLayerConstellation::StarType> MapLayerConstellation::generateStarPattern() const
{
	std::vector<StarType> c;

	for ( int y = 0; y < height_; ++y )
	{
		for ( int x = 0; x < width_; ++x )
		{
			bool draw_star = mezun::testRandomWithinPercentage( STAR_CHANCE );

			if ( draw_star )
			{
				int star_type_percent = mezun::randPercent();
				bool bright_star = mezun::testRandomWithinPercentage( BRIGHT_STAR_CHANCE );

				int star_type = -1;

				if ( BIG_STAR_CHANCE > star_type_percent )
				{
					star_type = ( int )StarType::BIG_STAR;
				}
				else if ( MEDIUM_STAR_CHANCE > star_type_percent )
				{
					star_type = ( int )StarType::MEDIUM_STAR;
				}
				else
				{
					star_type = ( int )StarType::SMALL_STAR;
				}

				if ( bright_star )
				{
					star_type += BRIGHT_STAR_MODIFIER;
				}

				c.push_back( ( StarType )star_type );
			}
			else
			{
				c.push_back( StarType::NO_STAR );
			}
		}
	}

	const int MOON_START_X = ( width_ - 2 );
	const int MOON_START_Y = 3;

	c.at( indexFromXAndY( MOON_START_X, MOON_START_Y ) ) = StarType::MOON_TOP_LEFT;
	c.at( indexFromXAndY( MOON_START_X + 1, MOON_START_Y ) ) = StarType::MOON_TOP_RIGHT;
	c.at( indexFromXAndY( MOON_START_X, MOON_START_Y + 1 ) ) = StarType::MOON_BOTTOM_LEFT;
	c.at( indexFromXAndY( MOON_START_X + 1, MOON_START_Y + 1 ) ) = StarType::MOON_BOTTOM_RIGHT;

	return c;
};

const int MapLayerConstellation::calculateWidth( int map_width_blocks ) const
{
	return std::max( ( int )( Unit::BlocksToMiniBlocks( map_width_blocks ) * scroll_speed_x_ ), Unit::WINDOW_WIDTH_MINIBLOCKS + 2);
};

const int MapLayerConstellation::calculateHeight( int map_height_blocks ) const
{
	return std::max( ( int )( Unit::BlocksToMiniBlocks( map_height_blocks ) * scroll_speed_y_ ), Unit::WINDOW_HEIGHT_MINIBLOCKS + 2 );
};

int MapLayerConstellation::indexFromXAndY( int x, int y ) const
{
	return ( ( y % height_ ) * width_ ) + ( x % width_ );
};