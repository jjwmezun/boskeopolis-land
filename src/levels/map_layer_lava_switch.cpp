#include "camera.hpp"
#include "event_system.hpp"
#include "level_state.hpp"
#include "map_layer_lava_switch.hpp"
#include "render.hpp"
#include "sprite.hpp"

#include <iostream>

static constexpr int MOVE_DOWN_SPEED = 2000;
static constexpr int MOVE_UP_SPEED = 800;

MapLayerLavaSwitch::MapLayerLavaSwitch( int y_off_blocks, int y_on_blocks, int x_block_when_lava_rises_forever, int layer_position )
:
    MapLayer( layer_position ),
    y_off_ ( Unit::BlocksToSubPixels( y_off_blocks ) ),
    y_on_ ( Unit::BlocksToSubPixels( y_on_blocks ) ),
    x_point_when_lava_rises_forever_ ( Unit::BlocksToSubPixels( x_block_when_lava_rises_forever ) ),
    layer_ ( y_off_blocks )
{};

MapLayerLavaSwitch::~MapLayerLavaSwitch() {};

void MapLayerLavaSwitch::update( LevelState& level_state )
{
    const int target_y = ( level_state.events().isSwitchOn() )
        ? y_on_
        : ( ( x_point_when_lava_rises_forever_ != -1 && level_state.sprites().hero().hit_box_.x > x_point_when_lava_rises_forever_ ) ? 0 : y_off_ );
    if ( layer_.getY() != target_y )
    {
        const int new_value = ( layer_.getY() + MOVE_DOWN_SPEED < target_y )
            ? layer_.getY() + MOVE_DOWN_SPEED
            : (
                  ( layer_.getY() - MOVE_UP_SPEED > target_y )
                  ? layer_.getY() - MOVE_UP_SPEED
                  : target_y
              );

        layer_.setY( new_value );
    }
    layer_.update( level_state );
};

void MapLayerLavaSwitch::render( const Camera& camera )
{
    layer_.render( camera );
};

void MapLayerLavaSwitch::interact( Sprite& sprite, LevelState& level_state )
{
    layer_.interact( sprite, level_state );
};