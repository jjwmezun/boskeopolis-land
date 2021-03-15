#include "camera.hpp"
#include "map_layer_lava.hpp"
#include "render.hpp"
#include "sprite.hpp"

MapLayerLava::MapLayerLava( int y_blocks, Unit::Layer layer_position )
:
    MapLayer( layer_position ),
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
        if ( animation_frame_ > 112 )
        {
            animation_frame_ = 0;
        }
        animation_timer_ = 0;
    }
};

void MapLayerLava::render( const Camera& camera )
{
    const int y = camera.relativeY( Unit::SubPixelsToPixels( y_ ) );
    sdl2::SDLRect r = { -( camera.x() % 16 ), y, 16, 272 };
    while ( r.x < 416 )
    {
        Render::renderObject( "bg/fire.png", { animation_frame_, 0, 16, 272 }, r );
        r.x += 16;
    }
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