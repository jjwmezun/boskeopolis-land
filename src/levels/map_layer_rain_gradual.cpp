#include "level_state.hpp"
#include "map.hpp"
#include "map_layer_rain_gradual.hpp"
#include "render.hpp"

MapLayerRainGradual::MapLayerRainGradual( int start, int end, Unit::Layer layer_position )
:
    MapLayer ( layer_position ),
    start_ ( ( start < 0 ) ? 0 : start ),
    max_ ( ( end < 0 ) ? -1 : end - start ),
    layer_ ( layer_position, 0 )
{};

MapLayerRainGradual::~MapLayerRainGradual() {};

void MapLayerRainGradual::update( LevelState& level_state )
{
    // ’Cause default max is dependent on map & map isn’t accessible during construction,
    // we have to set default max here.
    if ( max_ < 0 )
    {
        max_ = level_state.currentMap().widthBlocks() - start_;
    }

    // Set # o’ drops based on player’s x position relative to start & finish.
    const double relative = ( double )( std::min( max_, std::max( 0, Unit::SubPixelsToBlocks( level_state.sprites().hero().hit_box_.x ) - start_ ) ) );
    const double percent = relative / ( double )( max_ );
    const int new_number_of_drops = ( int )( std::max( 0.0, std::min( 255.0, percent * ( double )( MapLayerRain::MAX_DROPS ) ) ) );

    // If new # o’ drops greater than current #, add drops up to new #.
    for ( int i = layer_.number_of_drops_; i < new_number_of_drops; ++i )
    {
        layer_.generateDrop( i );
    }

    // If new # o’ drops lower than current #, remove drops till down to new #.
    // Note that we don’t need to remove the actual drops, but do need to remove
    // the entry in the rain map so that entry can be used by ’nother drop.
    for ( int i = new_number_of_drops; i < layer_.number_of_drops_; ++i )
    {
        layer_.removeDrop( i );
    }

    // Set current # to new #.
    layer_.number_of_drops_ = new_number_of_drops;

    layer_.update( level_state );
};

void MapLayerRainGradual::render( const Camera& camera )
{
    layer_.render( camera );
};