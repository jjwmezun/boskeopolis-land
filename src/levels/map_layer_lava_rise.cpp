#include "map_layer_lava_rise.hpp"

MapLayerLavaRise::MapLayerLavaRise( int y, int rise_speed, Unit::Layer layer_position )
:
    MapLayer( layer_position ),
    rise_speed_ ( rise_speed ),
    layer_ ( y, layer_position )
{};

MapLayerLavaRise::~MapLayerLavaRise() {};

void MapLayerLavaRise::update( LevelState& level_state )
{
    layer_.setY( layer_.getY() - rise_speed_ );
    layer_.update( level_state );
};

void MapLayerLavaRise::render( const Camera& camera )
{
    layer_.render( camera );
};

void MapLayerLavaRise::interact( Sprite& sprite, LevelState& level_state )
{
    layer_.interact( sprite, level_state );
};