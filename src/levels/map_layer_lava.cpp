#include "camera.hpp"
#include "map_layer_lava.hpp"
#include "render.hpp"
#include "sprite.hpp"

MapLayerLava::MapLayerLava( int y_blocks )
:
    y_ ( Unit::BlocksToSubPixels( y_blocks ) )
{};

MapLayerLava::~MapLayerLava() {};

void MapLayerLava::update( LevelState& level_state )
{
};

void MapLayerLava::render( const Camera& camera )
{
    const int y = camera.relativeY( Unit::SubPixelsToPixels( y_ ) );
    const sdl2::SDLRect r = { 0, y, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS - y };
    Render::renderRect( r, 4 );
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