#include "level_state.hpp"
#include "map_layer_overlay_transition.hpp"
#include "render.hpp"
#include "sprite.hpp"
#include "sprite_system.hpp"

MapLayerOverlayTransition::MapLayerOverlayTransition( Palette from, Palette to, int start, int end, Unit::Layer layer_position )
:
	MapLayer( layer_position ),
    max_ ( ( end == -1 ) ? -1 : end - start ),
    start_ ( start ),
    from_ ( from ),
    to_ ( to ),
    set_ ( false )
{};

MapLayerOverlayTransition::~MapLayerOverlayTransition() {};

void MapLayerOverlayTransition::render( const Camera& camera )
{
	Render::colorCanvasMultiply( red_, green_, blue_, alpha_ );
};

void MapLayerOverlayTransition::update( LevelState& level_state )
{
    if ( !set_ )
    {
        if ( max_ == -1 )
        {
            max_ = level_state.currentMap().widthBlocks() - start_;
        }
        Render::turnOnTransitionPalette( from_, to_ );
        set_ = true;
    }
    const double relative = ( double )( std::min( max_, std::max( 0, Unit::SubPixelsToBlocks( level_state.sprites().hero().hit_box_.x ) - start_ ) ) );
    const double percent = relative / ( double )( max_ );
    const Uint8 alpha = ( Uint8 )( std::max( 0.0, std::min( 255.0, percent * 255.0 ) ) );
    Render::setPaletteTransAlpha( 0 );
};
