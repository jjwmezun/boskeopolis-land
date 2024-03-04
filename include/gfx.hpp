#ifndef GFX_H
#define GFX_H

#include "arg.hpp"
#include <cstdint>

namespace BSL::GFX
{
    typedef void ( * input_handle_t )( void *, int, int, int, int );

    struct RGBColor
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };

    static constexpr unsigned int COLORS_PER_PALETTE = 9;

    int init();
    void close();
    void update( float dt );
    int testHasClosed();
    void handleEvents();
    double getTime();
    void registerInputHandler( input_handle_t handler );
    void setCanvasOpacity( float o );
    void setState( uint_fast8_t s );
    void clearGraphics();
    void clearStateGraphics();
    void removeGraphic( unsigned int id );
    void setPalette( const std::string & palname );

    struct Graphic
    {
        void setLayer( BSL::Layer layer );
        void setOpacity( float opacity );
        unsigned int id_;
    };

    struct Sprite : public Graphic
    {
        void setX( int v );
        void setY( int v );
        void setSrcX( uint_fast16_t v );
        void setSrcY( uint_fast16_t v );
        void setFlipX( bool v );
        void toggleFlipX();
    };

    struct RawSprite : public Graphic
    {
        void setY( int v );
        void setSrcX( unsigned int v );
        void setSrcY( unsigned int v );
    };

    struct Tilemap
    {
        void removeTile( uint_fast32_t i );
        void setY( int v );
        unsigned int id_;
    };

    struct Rect : public Graphic
    {
        void setY( int v );
    };

    struct RectGradient : public Graphic
    {
    };

    struct Text : Graphic
    {
        void changeText( const char * text, BSL::ArgList args );
        void setColor( uint_fast8_t color );
    };

    struct Counter : Graphic
    {
        void changeNumber( int_fast32_t n );
    };

    struct Timer : Graphic
    {
        void changeSeconds( uint_fast16_t seconds );
    };

    struct Menu : public Graphic
    {

    };

    struct Wallpaper : public Graphic
    {

    };

    struct Tile
    {
        bool set;
        uint_fast8_t x;
        uint_fast8_t y;
        uint_fast8_t animation;
        uint_fast8_t frame;
    };

    Rect addGraphicRect
    (
        int x,
        int y,
        unsigned int w,
        unsigned int h,
        unsigned char color,
        BSL::ArgList args = {}
    );

    RectGradient addGraphicRectGradient
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
        BSL::ArgList args = {}
    );

    RawSprite addGraphicSpriteRaw
    (
        const unsigned char * data,
        unsigned int dataw,
        unsigned int datah,
        unsigned int w,
        unsigned int h,
        int x,
        int y,
        BSL::ArgList args = {}
    );

    Text addGraphicText
    (
        const char * text,
        BSL::ArgList args = {}
    );

    Counter addGraphicCounter
    (
        int_fast32_t num,
        uint_fast8_t digits,
        BSL::ArgList args = {}
    );

    Timer addGraphicTimer
    (
        uint_fast16_t seconds,
        BSL::ArgList args = {}
    );

    Menu addGraphicMenu
    (
        unsigned int w,
        unsigned int h,
        int x,
        int y,
        BSL::ArgList args = {}
    );

    Wallpaper addGraphicWallpaper
    (
        uint_fast16_t texture,
        BSL::ArgList args = {}
    );

    unsigned int loadFileAsTexture( const char * filename );

    unsigned int getCameraX();
    unsigned int getCameraY();
    void setCameraX( unsigned int v );
    void setCameraY( unsigned int v );
}

#endif // GFX_H