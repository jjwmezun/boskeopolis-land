#include "event_system.hpp"
#include "level_state.hpp"
#include "map_layer_overlay.hpp"
#include "render.hpp"

MapLayerOverlay::MapLayerOverlay( Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha, OverlayType type, Unit::Layer layer_position )
:
	MapLayer( layer_position ),
	red_ ( red ),
	green_ ( green ),
	blue_ ( blue ),
	alpha_ ( alpha ),
	type_ ( type )
{};

MapLayerOverlay::~MapLayerOverlay() {};

void MapLayerOverlay::render( const Camera& camera )
{
	switch ( type_ )
	{
		case ( OverlayType::NORMAL ):
		{
			Render::colorCanvasForceColor( red_, green_, blue_, alpha_ );
		}
		break;
		case ( OverlayType::MULTIPLY ):
		{
			Render::colorCanvasMultiply( red_, green_, blue_, alpha_ );
		}
		break;
		case ( OverlayType::LIGHTEN ):
		{
			Render::colorCanvasBlend( red_, green_, blue_, alpha_, SDL_BLENDMODE_ADD );
		}
		break;
	}
};

void MapLayerOverlay::update( LevelState& level_state ) {};
