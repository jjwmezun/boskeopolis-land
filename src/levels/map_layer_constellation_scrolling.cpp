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
static int calculateHeight( int map_width_blocks, int map_height_blocks );

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
	src_ ( 0, 0, Unit::BlocksToPixels( map_width_blocks ), calculateHeight( map_width_blocks, map_height_blocks ) ),
	image_ ( image ),
	scroll_speed_ ( scrollSpeedX( map_width_blocks ) ),
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
		texture_ = MapLayerConstellation::formTexture( src_, image_ );
	}
	else
	{
		dest_.x = -( ( int )( ( double )( camera.x() ) * scroll_speed_ ) );
		dest_.y = -( ( int )( ( double )( camera.y() ) * scroll_speed_ ) );
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

int calculateHeight( int map_width_blocks, int map_height_blocks )
{
	return Unit::MiniBlocksToPixels( ( int )( ( double )( Unit::BlocksToMiniBlocks( map_height_blocks ) ) / scrollSpeedX( map_width_blocks ) ) );
};
