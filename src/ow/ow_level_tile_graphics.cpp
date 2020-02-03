#include "ow_level_tile_graphics.hpp"
#include "render.hpp"

OWLevelTileGraphics::OWLevelTileGraphics()
:
    prev_camera_x_ ( -1 ),
    prev_camera_y_ ( -1 ),
    animation_timer_ (),
    animation_frame_ (),
    tileset_ ( "tilesets/ow.png" ),
    destinations_ (),
    source_ ( 0, 240, 16, 16 ),
    positions_ (),
    reveal_ ()
{
    for ( int i = 0; i < Level::NUMBER_OF_LEVELS; ++i )
    {
        reveal_[ i ] = false;
        positions_[ i ] = { 0, 0, 8, 8 };
    }
};

void OWLevelTileGraphics::add( sdl2::SDLRect dest, int level, bool reveal )
{
    reveal_[ level ] = reveal;
    positions_[ level ] = dest;
};

void OWLevelTileGraphics::update( const sdl2::SDLRect& camera )
{
    if ( prev_camera_x_ != camera.x || prev_camera_y_ != camera.y )
    {
        refreshGraphics( camera );
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

void OWLevelTileGraphics::showTile( const sdl2::SDLRect& camera, int level )
{
    reveal_[ level ] = true;
    refreshGraphics( camera );
};

void OWLevelTileGraphics::refreshGraphics( const sdl2::SDLRect& camera )
{
    destinations_.clear();
    for ( int i = 0; i < Level::NUMBER_OF_LEVELS; ++i )
    {
        const auto& position = positions_[ i ];
        if ( reveal_[ i ] && testOnCamera( position, camera ) )
        {
            destinations_.push_back({ position.x - camera.x, position.y - camera.y, 16, 16 });
        }
    }
};