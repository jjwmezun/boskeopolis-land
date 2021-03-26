#include "event_system.hpp"
#include "level_state.hpp"
#include "map_layer_overlay.hpp"
#include "render.hpp"

MapLayerOverlay::MapLayerOverlay( Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha, Unit::Layer layer_position )
:
	MapLayer( layer_position ),
	red_ ( red ),
	green_ ( green ),
	blue_ ( blue ),
	alpha_ ( 0 )
{};

MapLayerOverlay::~MapLayerOverlay() {};

void MapLayerOverlay::render( const Camera& camera )
{
	Render::colorCanvasMultiply( red_, green_, blue_, alpha_ );
};

void MapLayerOverlay::update( LevelState& level_state ) {};
