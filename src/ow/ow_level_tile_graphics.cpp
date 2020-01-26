#include "ow_level_tile_graphics.hpp"
#include "render.hpp"

OWLevelTileGraphics::OWLevelTileGraphics()
:
    prev_camera_x_ ( -1 ),
    prev_camera_y_ ( -1 ),
    animation_timer_ (),
    animation_frame_ (),
    tileset_ ( "tilesets/ow.png" ),
    positions_ (),
    destinations_ (),
    source_ ( 0, 240, 16, 16 )
{};

void OWLevelTileGraphics::add( sdl2::SDLRect dest )
{
    positions_.emplace_back( dest );
};

void OWLevelTileGraphics::update( const sdl2::SDLRect& camera )
{
    if ( prev_camera_x_ != camera.x || prev_camera_y_ != camera.y )
    {
        destinations_.clear();
        for ( const auto& position : positions_ )
        {
            if ( testOnCamera( position, camera ) )
            {
                destinations_.push_back({ position.x - camera.x, position.y - camera.y, 16, 16 });
            }
        }
    }
    prev_camera_x_ = camera.x;
    prev_camera_y_ = camera.y;

    if ( animation_timer_.update() )
    {
        ++animation_frame_;
        source_.x = Unit::BlocksToPixels( ANIMATION_FRAMES[ animation_frame_() ] );
    }
};

void OWLevelTileGraphics::render() const
{
    for ( const auto& dest : destinations_ )
    {
        Render::renderObject( tileset_, source_, dest );
    }
};

bool OWLevelTileGraphics::testOnCamera( const sdl2::SDLRect& position, const sdl2::SDLRect& camera ) const
{
    return
        position.right() > camera.x &&
        position.x < camera.x + Unit::WINDOW_WIDTH_PIXELS &&
        position.bottom() > camera.y &&
        position.y < camera.y + Unit::WINDOW_HEIGHT_PIXELS;
};