#include "render.hpp"
#include "texture_box.hpp"

TextureBox::TextureBox()
:
    texture_ ( nullptr ),
    dest_ ( { 0, 0, 0, 0 } )
{};

void TextureBox::init()
{
    texture_ = Render::createRenderBox();
}

void TextureBox::startDrawing()
{
    Render::setRenderTarget( texture_ );
};

void TextureBox::endDrawing()
{
    Render::releaseRenderTarget();
};

void TextureBox::render() const
{
    Render::renderRenderBox( texture_ );
};

void TextureBox::destroy()
{
    if ( texture_ != nullptr )
    {
	    SDL_DestroyTexture( texture_ );
    }
};