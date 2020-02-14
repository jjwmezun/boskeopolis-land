#include "event_system.hpp"
#include "map_layer_shade.hpp"
#include "render.hpp"

MapLayerShade::MapLayerShade( int shade, int alpha )
:
	shade_ ( shade ),
	alpha_ ( 0 ),
	timer_ ( 0 )
{};

MapLayerShade::~MapLayerShade() {};

void MapLayerShade::render( const Camera& camera )
{
	Render::colorCanvas( shade_, alpha_ );
};

void MapLayerShade::update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap, const SpriteSystem& sprites )
{
	if ( events.testLightSwitch() )
	{
		alpha_ = 0;
		timer_ = 0;
	}
	else if ( alpha_ < 200 )
	{
		++timer_;
		alpha_ = ( Uint8 )( ( double )( timer_ ) / 2.0 );
	}
};
