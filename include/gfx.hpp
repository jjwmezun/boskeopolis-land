#ifndef GFX_H
#define GFX_H

#include "arg.hpp"
#include <cstdint>

namespace BSL::GFX
{
    typedef void ( * input_handle_t )( void *, int, int, int, int );

    int init();
    void close();
    void update( float dt );
    int testHasClosed();
    void handleEvents();
    double getTime();
    void registerInputHandler( input_handle_t handler );
    void setPalette( unsigned char p );

    struct Graphic
    {
        void setLayer( BSL::Layer layer );
        unsigned int id_;
    };

    struct Sprite : public Graphic
    {
        void setX( int v );
        void setY( int v );
        void setSrcX( int v );
    };

    struct Tilemap
    {
        void setY( int v );
        unsigned int id_;
    };

    struct Tile
    {
        bool set;
        uint_fast8_t x;
        uint_fast8_t y;
        uint_fast8_t animation;
        uint_fast8_t frame;
    };

    int addGraphicRect
    (
        int x,
        int y,
        unsigned int w,
        unsigned int h,
        unsigned char color,
        BSL::ArgList args = {}
    );

    int addGraphicRectGradient
    (
        int x,
        int y,
        unsigned int w,
        unsigned int h,
        unsigned char color1,
        unsigned char color2,
        BSL::ArgList args = {}
    );

    Sprite addGraphicSprite
    (
        unsigned int texture,
        int x,
        int y,
        unsigned int w,
        unsigned int h,
        BSL::ArgList args = {}
    );

    Tilemap addGraphicTilemap
    (
        unsigned int texture,
        Tile * tiles,
        unsigned int w,
        unsigned int h,
        int x,
        int y,
        BSL::ArgList args = {}
    );

    int addGraphicSpriteRaw
    (
        uint_fast8_t abs,
        unsigned char * data,
        unsigned int dataw,
        unsigned int datah,
        unsigned int srcx,
        unsigned int srcy,
        unsigned int w,
        unsigned int h,
        int x,
        int y
    );

    int addGraphicText
    (
        const char * text
    );

    int loadFileAsTexture( const char * filename );

    void setCameraX( unsigned int v );
    void setCameraY( unsigned int v );
}

#endif // GFX_H