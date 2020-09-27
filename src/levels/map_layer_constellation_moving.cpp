#include "map_layer_constellation.hpp"
#include "map_layer_constellation_moving.hpp"
#include "render.hpp"
#include "unit.hpp"

// MEMBER FUNCTIONS
//=============================================================

MapLayerConstellationMoving::MapLayerConstellationMoving( int width, int height, double move_speed, std::string image, int layer_position )
:
	MapLayer( layer_position ),
	src_ ( 0, 0, width, height ),
	image_ ( image ),
	move_speed_ ( abs( move_speed ) ),
	texture_ ( nullptr )
{
	dest2_ = dest_ = src_;
};

MapLayerConstellationMoving::~MapLayerConstellationMoving()
{
	SDL_DestroyTexture( texture_ );
};

void MapLayerConstellationMoving::update( LevelState& level_state )
{
	if ( texture_ == nullptr )
	{
		texture_ = MapLayerConstellation::formTexture( src_, image_ );
	}
	else
	{
		dest_.x -= ( int )( floor( move_speed_ ) );
		while ( dest_.x < -dest_.w )
		{
			dest_.x += dest_.w;
		}
		dest2_.x = dest_.w + dest_.x;
	}
};

void MapLayerConstellationMoving::render( const Camera& camera )
{
	if ( texture_ != nullptr )
	{
		Render::renderObject( texture_, src_, dest_ );
		if ( dest2_.x < Unit::WINDOW_WIDTH_PIXELS )
		{
			Render::renderObject( texture_, src_, dest2_ );
		}
	}
};
