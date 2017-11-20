#include "camera.hpp"
#include <cassert>
#include <iostream>
#include "main.hpp"
#include "map_layer_constellation.hpp"
#include "render.hpp"
#include "unit.hpp"


// CONSTANTS
//=============================================================

static constexpr int BRIGHT_STAR_MODIFIER = 3;
static constexpr int TILE_SIZE = Unit::PIXELS_PER_MINIBLOCK;
static constexpr int STAR_CHANCE = 25;
static constexpr int BIG_STAR_CHANCE = 10;
static constexpr int MEDIUM_STAR_CHANCE = 25;
static constexpr int BRIGHT_STAR_CHANCE = 25;
static constexpr int MOON_START_X = Unit::WINDOW_WIDTH_MINIBLOCKS + 6;
static constexpr int MOON_START_Y = 3;
static constexpr int WIDTH = MOON_START_X - 2 + Unit::WINDOW_WIDTH_MINIBLOCKS;
static constexpr int WIDTH_PIXELS = Unit::MiniBlocksToPixels( WIDTH );



// CONSTRUCTOR FUNCTION DECLARATIONS
//=============================================================

static double scrollSpeedX( int map_width_blocks );
static int calculateHeight( int map_height_blocks, double scroll_speed );
static const std::vector<StarType> generateStarPattern( int height );
static const std::string graphicsPath();



// MEMBER FUNCTIONS
//=============================================================

MapLayerConstellation::MapLayerConstellation
(
	int map_width_blocks,
	int map_height_blocks
)
:
	MapLayer()
{
	const double scroll_speed = scrollSpeedX( map_width_blocks );
	const int map_width_miniblocks = Unit::BlocksToMiniBlocks( map_width_blocks );
	const int map_height_miniblocks = Unit::BlocksToMiniBlocks( map_height_blocks );
	const int height = calculateHeight( map_height_miniblocks, scroll_speed );
	const int height_pixels = Unit::MiniBlocksToPixels( height );
	std::vector<StarType> star_pattern = generateStarPattern( height );

	scroll_speed_ = scroll_speed;
	texture_ = Render::createRenderBox( WIDTH_PIXELS, height_pixels );
	height_ = height;
	rendered_ = false;
	src_ = { 0, 0, WIDTH_PIXELS, height_pixels };
	star_pattern_ = std::move( star_pattern );
};

MapLayerConstellation::~MapLayerConstellation()
{
	SDL_DestroyTexture( texture_ );
};

void MapLayerConstellation::update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap )
{
	if ( !rendered_ )
	{
		formTexture();
	}
};

void MapLayerConstellation::render( const Camera& camera )
{
	const sdl2::SDLRect dest =
	{
		-( ( int )( ( double )( camera.x() ) * scroll_speed_ ) ),
		-( ( int )( ( double )( camera.y() ) * scroll_speed_ ) ),
		src_.w,
		src_.h
	};
	Render::renderObject( texture_, src_, dest );
};

void MapLayerConstellation::renderTexture()
{
	const std::string img = graphicsPath();
	sdl2::SDLRect source = { 0, 0, TILE_SIZE, TILE_SIZE };
	sdl2::SDLRect dest = { 0, 0, TILE_SIZE, TILE_SIZE };

	for ( int y = 0; y < height_; ++y )
	{
		dest.x = 0;

		for ( int x = 0; x < WIDTH; ++x )
		{
			const int index = mezun::nOfXY( x, y, WIDTH );
			assert( index < star_pattern_.size() );

			if ( star_pattern_.at( index ) != StarType::NO_STAR )
			{
				source.x = ( int )( star_pattern_.at( index ) ) * TILE_SIZE;
				Render::renderObjectNoMagnify( img, source, dest );
			}

			dest.x += TILE_SIZE;
		}

		dest.y += TILE_SIZE;
	}
};

void MapLayerConstellation::formTexture()
{
	Render::setRenderTarget( texture_ );
		Render::clearScreenTransparency();
		renderTexture();
	Render::releaseRenderTarget();
	rendered_ = true;
};

void MapLayerConstellation::testValues() const
{
	std::cout<<"STARS: "<<star_pattern_.size()<<std::endl;
	std::cout<<"SRC: "<<src_.w<<std::endl;
	std::cout<<"SCROLL: "<<scroll_speed_<<std::endl;
	std::cout<<"H: "<<height_<<std::endl;
	std::cout<<"RENDERERED: "<<rendered_<<std::endl;
	std::cout<<"TEXTURE: "<<texture_<<std::endl;
};



// CONSTRUCTOR FUNCTIONS
//=============================================================

double scrollSpeedX( int map_width_blocks )
{
	return ( double )( WIDTH ) / ( double )( map_width_blocks * 4 );
};

int calculateHeight( int map_height_miniblocks, double scroll_speed )
{
	return ( int )( ( double )( map_height_miniblocks ) / scroll_speed );
};

const std::vector<StarType> generateStarPattern( int height )
{
	std::vector<StarType> c;

	for ( int y = 0; y < height; ++y )
	{
		for ( int x = 0; x < WIDTH; ++x )
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

	c.at( mezun::nOfXY( MOON_START_X, MOON_START_Y, WIDTH ) ) = StarType::MOON_TOP_LEFT;
	c.at( mezun::nOfXY( MOON_START_X + 1, MOON_START_Y, WIDTH ) ) = StarType::MOON_TOP_RIGHT;
	c.at( mezun::nOfXY( MOON_START_X, MOON_START_Y + 1, WIDTH ) ) = StarType::MOON_BOTTOM_LEFT;
	c.at( mezun::nOfXY( MOON_START_X + 1, MOON_START_Y + 1, WIDTH ) ) = StarType::MOON_BOTTOM_RIGHT;

	return c;
};

const std::string graphicsPath()
{
	return "bg" + Main::pathDivider() + "constellation.png";
};