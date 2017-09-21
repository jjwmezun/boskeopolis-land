#include "event_system.hpp"
#include "map_layer_shade.hpp"
#include "render.hpp"

MapLayerShade::MapLayerShade( int shade, int alpha ) : shade_ ( shade ), alpha_ ( 0 ) {};
MapLayerShade::~MapLayerShade() {};

void MapLayerShade::render( const Camera& camera ) const
{
	Render::colorCanvas( shade_, alpha_ );
};

void MapLayerShade::update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap )
{
	if ( events.testLightSwitch() )
	{
		alpha_ = 0;
	}
	else if ( alpha_ < 236 )
	{
		++alpha_;
	}
};