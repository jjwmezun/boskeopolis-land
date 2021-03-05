#include "camera.hpp"
#include "level_state.hpp"
#include "map_layer_constellation.hpp"
#include "map_layer_constellation_scrolling.hpp"
#include "render.hpp"
#include "unit.hpp"


// CONSTANTS
//=============================================================

static constexpr int WIDTH = ( Unit::WINDOW_WIDTH_MINIBLOCKS * 2 ) + 4;
static constexpr int WIDTH_PIXELS = Unit::MiniBlocksToPixels( WIDTH );

// CONSTRUCTOR FUNCTION DECLARATIONS
//=============================================================

static double scrollSpeedX( int map_width_blocks );

// MEMBER FUNCTIONS
//=============================================================

MapLayerConstellationScrolling::MapLayerConstellationScrolling
(
	int map_width_blocks,
	int map_height_blocks,
	std::string image,
	Unit::Layer layer_position
)
:
	MapLayer( layer_position ),
	src_ ( 0, 0, Unit::BlocksToPixels( map_width_blocks ), Unit::BlocksToPixels( map_height_blocks ) ),
	image_ ( image ),
	scroll_speed_x_ ( scrollSpeedX( map_width_blocks ) ),
	scroll_speed_y_ ( 0.0 ),
	texture_ ( nullptr )
{
	dest_ = src_;
};

MapLayerConstellationScrolling::~MapLayerConstellationScrolling()
{
	SDL_DestroyTexture( texture_ );
};

void MapLayerConstellationScrolling::update( LevelState& level_state )
{
	const Camera& camera = level_state.camera();
	if ( texture_ == nullptr )
	{
		const int height = level_state.currentMap().heightPixels();
		scroll_speed_y_ = ( height == dest_.h ) ? 0.0 : ( double )( dest_.h ) / ( double )( height );
		if ( src_.h < Unit::WINDOW_HEIGHT_PIXELS )
		{
			src_.h = dest_.h = Unit::WINDOW_HEIGHT_PIXELS;
		}

		texture_ = MapLayerConstellation::formTexture( src_, image_ );
	}
	else
	{
		dest_.x = -( ( int )( ( double )( camera.x() ) * scroll_speed_x_ ) );
		dest_.y = -( ( int )( ( double )( camera.y() ) * scroll_speed_y_ ) );
	}
};

void MapLayerConstellationScrolling::render( const Camera& camera )
{
	if ( texture_ != nullptr )
	{
		Render::renderObject( texture_, src_, dest_ );
	}
};

// CONSTRUCTOR FUNCTIONS
//=============================================================

double scrollSpeedX( int map_width_blocks )
{
	return ( double )( WIDTH ) / ( double )( map_width_blocks * 4 );
};
