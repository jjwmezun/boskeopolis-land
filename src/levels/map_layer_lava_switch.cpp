#include "camera.hpp"
#include "event_system.hpp"
#include "level_state.hpp"
#include "map_layer_lava_switch.hpp"
#include "render.hpp"
#include "sprite.hpp"

#include <iostream>

static constexpr int MOVE_SPEED = 1000;

MapLayerLavaSwitch::MapLayerLavaSwitch( int y_off_blocks, int y_on_blocks )
:
    y_off_ ( Unit::BlocksToSubPixels( y_off_blocks ) ),
    y_on_ ( Unit::BlocksToSubPixels( y_on_blocks ) ),
    layer_ ( y_off_blocks )
{};

MapLayerLavaSwitch::~MapLayerLavaSwitch() {};

void MapLayerLavaSwitch::update( LevelState& level_state )
{
    const int target_y = ( level_state.events().isSwitchOn() )
        ? y_on_
        : ( ( level_state.sprites().hero().hit_box_.x > Unit::BlocksToSubPixels( 102 ) ) ? 0 : y_off_ );
    if ( layer_.getY() != target_y )
    {
        const int new_value = ( layer_.getY() + MOVE_SPEED < target_y )
            ? layer_.getY() + MOVE_SPEED
            : (
                  ( layer_.getY() - MOVE_SPEED > target_y )
                  ? layer_.getY() - MOVE_SPEED
                  : target_y
              );

        layer_.setY( new_value );
    }
};

void MapLayerLavaSwitch::render( const Camera& camera )
{
    layer_.render( camera );
};

void MapLayerLavaSwitch::interact( Sprite& sprite, LevelState& level_state )
{
    layer_.interact( sprite, level_state );
};