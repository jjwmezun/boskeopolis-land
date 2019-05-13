#include "map_layer_constellation.hpp"
#include "map_layer_constellation_moving.hpp"
#include "render.hpp"
#include "unit.hpp"

// MEMBER FUNCTIONS
//=============================================================

MapLayerConstellationMoving::MapLayerConstellationMoving( int width, int height, double move_speed )
:
	MapLayer(),
	src_ ( 0, 0, width, height ),
	move_speed_ ( abs( move_speed ) ),
	texture_ ( nullptr )
{
	dest2_ = dest_ = src_;
};

MapLayerConstellationMoving::~MapLayerConstellationMoving()
{
	SDL_DestroyTexture( texture_ );
};

void MapLayerConstellationMoving::update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap, const SpriteSystem& sprites )
{
	if ( texture_ == nullptr )
	{
		texture_ = MapLayerConstellation::formTexture( src_ );
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
