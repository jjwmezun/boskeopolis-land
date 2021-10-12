#include <cassert>
#include "render.hpp"
#include "texture_box.hpp"

TextureBox::TextureBox( int width, int height, int x, int y )
:
    texture_ ( nullptr ),
    dest_ ( { x, y, width, height } ),
    src_ ( 0, 0, width, height ),
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
    src_ = t.src_;
    token_ = t.token_;
    return *this;
};

TextureBox::TextureBox( TextureBox&& t )
:
    texture_ ( t.texture_ ),
    dest_ ( t.dest_ ),
    src_ ( t.src_ ),
    token_ ( t.token_ )
{};

void TextureBox::init()
{
    texture_ = Render::createRenderBox( dest_.w, dest_.h );
    token_ = true;
}

void TextureBox::changeSize( int width, int height )
{
    dest_.w = src_.w = width;
    dest_.h = src_.h = height;
};

void TextureBox::startDrawing() const
{
    Render::setRenderTarget( texture_ );
};

void TextureBox::endDrawing() const
{
    Render::releaseRenderTarget();
};

void TextureBox::render() const
{
    Render::renderRenderBox( texture_, src_, dest_ );
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

void TextureBox::moveLeft( int amount )
{
    dest_.x -= amount;
};

void TextureBox::moveRight( int amount )
{
    dest_.x += amount;
};

void TextureBox::moveUp( int amount )
{
    dest_.y -= amount;
    
};

void TextureBox::moveDown( int amount )
{
    dest_.y += amount;
};

void TextureBox::setX( int value )
{
    dest_.x = value;
};

void TextureBox::setY( int value )
{
    dest_.y = value;
};

int TextureBox::getX() const
{
    return dest_.x;
};

int TextureBox::getY() const
{
    return dest_.y;
};

const sdl2::SDLRect& TextureBox::getDest() const
{
    return dest_;
};

const sdl2::SDLRect& TextureBox::getSrc() const
{
    return src_;
};

SDL_Texture* TextureBox::getTexture() const
{
    return texture_;
};

void TextureBox::setHeight( int value )
{
    src_.h = dest_.h = value;
};

int TextureBox::getHeight() const
{
    return src_.h;
};

void TextureBox::setWidth( int value )
{
    src_.w = dest_.w = value;
};

int TextureBox::getWidth() const
{
    return src_.w;
};

void TextureBox::setAlpha( Uint8 alpha )
{
    SDL_SetTextureAlphaMod( texture_, alpha );
}