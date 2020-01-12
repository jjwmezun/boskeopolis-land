#include "image_graphics.hpp"
#include "render.hpp"

ImageGraphics::ImageGraphics( std::string image, sdl2::SDLRect src, sdl2::SDLRect dest )
:
    image_ ( image ),
    src_ ( src ),
    dest_ ( dest )
{};

ImageGraphics::ImageGraphics( std::string image, sdl2::SDLRect rect )
:
    image_ ( image ),
    src_ ( rect ),
    dest_ ( rect )
{};

void ImageGraphics::render() const
{
    Render::renderObject( image_, src_, dest_ );
};