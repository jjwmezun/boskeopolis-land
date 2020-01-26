#include "ow_camera_arrows.hpp"

#include "unit.hpp"

OWCameraArrows::OWCameraArrows()
:
    show_arrows_ ( false ),
    prev_camera_x_ ( -1 ),
    prev_camera_y_ ( -1 ),
    animation_timer_ (),
    graphics_ ( "bg/overworld-arrows.png", { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS }, { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS } )
{};

void OWCameraArrows::update( const sdl2::SDLRect& camera )
{
    animation_timer_.update();
    show_arrows_ = testShowArrows( camera );
    prev_camera_x_ = camera.x;
    prev_camera_y_ = camera.y;
};

void OWCameraArrows::render() const
{
    if ( show_arrows_ )
    {
        graphics_.render();
    }
};

bool OWCameraArrows::testShowArrows( const sdl2::SDLRect& camera ) const
{
    return animation_timer_.value() > 16
        || prev_camera_x_ != camera.x
        || prev_camera_y_ != camera.y;
};