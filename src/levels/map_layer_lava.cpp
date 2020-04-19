#include "camera.hpp"
#include "map_layer_lava.hpp"
#include "render.hpp"
#include "sprite.hpp"

#include <iostream>

MapLayerLava::MapLayerLava( int y_blocks )
:
    y_ ( Unit::BlocksToSubPixels( y_blocks ) ),
    animation_timer_ ( 0 ),
    animation_frame_ ( 0 )
{};

MapLayerLava::~MapLayerLava() {};

void MapLayerLava::update( LevelState& level_state )
{
    ++animation_timer_;
    if ( animation_timer_ == 8 )
    {
        animation_frame_ += 16;
        if ( animation_frame_ > 16 )
        {
            animation_frame_ = 0;
        }
        animation_timer_ = 0;
    }
};

void MapLayerLava::render( const Camera& camera )
{
    const int x = -( camera.x() % 256 );
    const int y = camera.relativeY( Unit::SubPixelsToPixels( y_ ) );
    sdl2::SDLRect r = { x, y, 800, 16 };
    Render::renderObject( "bg/fire.png", { 0, animation_frame_, 800, 16 }, r );
    r = { x, y + 16, 800, 256 };
    Render::renderObject( "bg/fire.png", { 0, 32, 800, 256 }, r );
};

void MapLayerLava::interact( Sprite& sprite, LevelState& level_state )
{
    if ( sprite.hasType( Sprite::SpriteType::HERO ) )
    {
        if ( sprite.hit_box_.y > y_ )
        {
            sprite.killNoAnimation();
        }
    }
};

void MapLayerLava::setY( int new_value )
{
    y_ = new_value;
};

int MapLayerLava::getY() const
{
    return y_;
};