#include <cassert>
#include "render.hpp"
#include "texture_box.hpp"

TextureBox::TextureBox()
:
    texture_ ( nullptr ),
    dest_ ( { 0, 0, 0, 0 } ),
    token_ ( false )
{};

TextureBox::~TextureBox()
{
    assert( !token_ );
};

TextureBox& TextureBox::operator=( TextureBox&& t )
{
    texture_ = t.texture_;
    dest_ = t.dest_;
    token_ = t.token_;
    return *this;
};

void TextureBox::init()
{
    texture_ = Render::createRenderBox();
    token_ = true;
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
        Render::clearRenderBox( texture_ );
	    SDL_DestroyTexture( texture_ );
    }
    token_ = false;
};