#include "config.hpp"
#include "nasringine/nasr.h"
#include "renderer.hpp"

#include <iostream>

namespace BSL
{
    Renderer::Renderer()
    {
        int charset = NasrAddCharset( "assets/graphics/charset/latin.png", "assets/charset/latin.json" );
        if ( charset < 0 )
        {
            // TODO: Throw exception
            std::cout << "NO CHARSET" << std::endl;
        }
        charset_ = static_cast<unsigned int>( charset );
    };

    unsigned int Renderer::addSprite
    (
        std::string texture,
        float srcx,
        float srcy,
        float w,
        float h,
        float x,
        float y,
        ArgList args
    ) const
    {
        std::string filename = "assets/graphics/" + texture;
        int texture_id = NasrLoadFileAsTexture( filename.c_str() );
        if ( texture_id < 0 )
        {
            // TODO: throw exception.
            std::cout << "NO TEXT" << std::endl;
        }

        uint_fast8_t abs = GetArgConvert<uint_fast8_t, bool> ( "abs", args, false );
        unsigned int layer = GetArgConvert<unsigned int, Layer> ( "layer", args, Layer::SPRITES_1 );
        uint_fast8_t flip_x = 0;
        uint_fast8_t flip_y = 0;
        float rotation_x = 0.0f;
        float rotation_y = 0.0f;
        float rotation_z = 0.0f;
        float opacity = 1.0f;

        NasrRect src { srcx, srcy, w, h };
        NasrRect dest { x, y, w, h };

        int graphic = NasrGraphicsAddSprite
        (
            abs,
            current_state_,
            static_cast<unsigned int>( layer ),
            texture_id,
            src,
            dest,
            flip_x,
            flip_y,
            rotation_x,
            rotation_y,
            rotation_z,
            opacity,
            0,
            1
        );

        if ( graphic < 0 )
        {
            // TODO: throw exception.
            std::cout << "NO GRAPH" << std::endl;
        }

        return static_cast<unsigned int>( graphic );
    };

    unsigned int Renderer::addRect
    (
        float x,
        float y,
        float w,
        float h,
        uint_fast8_t color,
        bool abs,
        Layer layer
    ) const
    {
        NasrRect r { x, y, w, h };
        int graphic = NasrGraphicsAddRectPalette
        (
            static_cast<uint_fast8_t>( abs ),
            current_state_,
            static_cast<unsigned int>( layer ),
            r,
            0,
            color,
            1
        );

        if ( graphic < 0 )
        {
            // TODO: throw exception.
            std::cout << "NO GRAPH" << std::endl;
        }

        return static_cast<unsigned int>( graphic );
    };

    unsigned int Renderer::addRectGradient
    (
        float x,
        float y,
        float w,
        float h,
        Dir::XY dir,
        uint_fast8_t color1,
        uint_fast8_t color2,
        bool abs,
        Layer layer
    ) const
    {
        NasrRect r { x, y, w, h };
        int graphic = NasrGraphicsAddRectGradientPalette
        (
            static_cast<uint_fast8_t>( abs ),
            current_state_,
            static_cast<unsigned int>( layer ),
            r,
            0,
            static_cast<uint_fast8_t>( dir ),
            color1,
            color2,
            1
        );

        if ( graphic < 0 )
        {
            // TODO: throw exception.
            std::cout << "NO GRAPH" << std::endl;
        }

        return static_cast<unsigned int>( graphic );
    };

    unsigned int Renderer::addTilemap
    (
        const std::string & tileset,
        const std::vector<NasrTile> & tiles,
        unsigned int width,
        unsigned height,
        ArgList args
    ) const
    {
        // Tileset.
        std::string tileset_file = "assets/graphics/tilesets/" + tileset + ".png";
        int tileset_id = NasrLoadFileAsTexture( tileset_file.c_str() );

        if ( tileset_id < 0 )
        {
            // TODO: throw exception.
            std::cout << "NO GRAPH" << std::endl;
        }

        unsigned int abs = 0;
        unsigned int layer = static_cast<int>( Layer::BLOCKS_1 );

        int graphic = NasrGraphicsAddTilemap
        (
            abs,
            current_state_,
            layer,
            tileset_id,
            &tiles[ 0 ],
            width,
            height,
            1
        );

        if ( graphic < 0 )
        {
            // TODO: throw exception.
            std::cout << "NO GRAPH" << std::endl;
        }

        return static_cast<unsigned int>( graphic );
    };

    TextGraphic Renderer::addText
    (
        std::string && text,
        uint_fast8_t color,
        float x,
        float y,
        float w,
        float h,
        ArgList args
    ) const
    {
        uint_fast8_t abs = GetArgConvert<uint_fast8_t, bool> ( "abs", args, true );
        unsigned int layer = GetArgConvert<unsigned int, Layer> ( "layer", args, Layer::AFTER_FG_2 );
        std::vector<char> cstring( text.c_str(), text.c_str() + text.size() + 1 );
        NasrRect coords { x, y, w, h };
        uint_fast8_t align = GetArgConvert<uint_fast8_t, Text::Align> ( "align", args, NASR_ALIGN_DEFAULT );
        uint_fast8_t valign = GetArgConvert<uint_fast8_t, Text::Valign> ( "valign", args, NASR_VALIGN_DEFAULT );
        float padding_left = GetArg<float> ( "leftPadding", args, 0.0f );
        float padding_right = GetArg<float> ( "rightPadding", args, 0.0f );
        float padding_top = GetArg<float> ( "topPadding", args, 0.0f );
        float padding_bottom = GetArg<float> ( "bottomPadding", args, 0.0f );
        float xoffset = GetArg<float> ( "xOffset", args, 0.0f );
        float yoffset = GetArg<float> ( "yPadding", args, 0.0f );
        float shadow = GetArg<float> ( "shadow", args, 0.0f );
        float opacity = GetArg( "opacity", args, 1.0f );

        NasrText t
        {
            &cstring[ 0 ],
            charset_,
            coords,
            align,
            valign,
            padding_left,
            padding_right,
            padding_top,
            padding_bottom,
            xoffset,
            yoffset,
            shadow,
            opacity
        };

        int graphic = NasrGraphicsAddTextPalette
        (
            abs,
            current_state_,
            layer,
            t,
            0,
            1,
            color
        );

        if ( graphic < 0 )
        {
            // TODO: throw exception.
            std::cout << "NO GRAPH" << std::endl;
        }

        return { static_cast<unsigned int>( graphic ) };
    };

    TextGraphic Renderer::addTextGradient
    (
        std::string && text,
        Dir::XY dir,
        uint_fast8_t color1,
        uint_fast8_t color2,
        float x,
        float y,
        float w,
        float h,
        ArgList args
    ) const
    {
        uint_fast8_t abs = 1;
        unsigned int layer = GetArgConvert<unsigned int, Layer> ( "layer", args, Layer::AFTER_FG_2 );
        std::vector<char> cstring( text.c_str(), text.c_str() + text.size() + 1 );
        NasrRect coords { x, y, w, h };
        uint_fast8_t align = GetArgConvert<uint_fast8_t, Text::Align> ( "align", args, NASR_ALIGN_DEFAULT );
        uint_fast8_t valign = GetArgConvert<uint_fast8_t, Text::Valign> ( "valign", args, NASR_VALIGN_DEFAULT );
        float padding_left = GetArg<float> ( "leftPadding", args, 0.0f );
        float padding_right = GetArg<float> ( "rightPadding", args, 0.0f );
        float padding_top = GetArg<float> ( "topPadding", args, 0.0f );
        float padding_bottom = GetArg<float> ( "bottomPadding", args, 0.0f );
        float xoffset = GetArg<float> ( "xOffset", args, 0.0f );
        float yoffset = GetArg<float> ( "yPadding", args, 0.0f );
        float shadow = GetArg<float> ( "shadow", args, 0.0f );
        uint_fast8_t dirval = static_cast<uint_fast8_t> ( dir );
        float opacity = GetArg( "opacity", args, 1.0f );

        NasrText t
        {
            &cstring[ 0 ],
            charset_,
            coords,
            align,
            valign,
            padding_left,
            padding_right,
            padding_top,
            padding_bottom,
            xoffset,
            yoffset,
            shadow,
            opacity
        };

        int graphic = NasrGraphicsAddTextGradientPalette
        (
            abs,
            current_state_,
            layer,
            t,
            0,
            1,
            dirval,
            color1,
            color2
        );

        if ( graphic < 0 )
        {
            // TODO: throw exception.
            std::cout << "NO GRAPH" << std::endl;
        }

        return { static_cast<unsigned int>( graphic ) };
    };
    
    SpriteGraphic Renderer::addMenuBox
    (
        Text::Align align,
        Text::Valign valign,
        float w,
        float h,
        ArgList args
    ) const
    {
        int texture = NasrAddTextureBlank( static_cast<unsigned int>( w ), static_cast<unsigned int>( h ) );
        if ( texture < 0 )
        {
            // TODO: Throw exception.
            std::cout << "NO MENU" << std::endl;
        }
        int menu_texture = NasrLoadFileAsTexture( "assets/graphics/misc/menu.png" );
        if ( menu_texture < 0 )
        {
            // TODO: Throw exception.
            std::cout << "NO MENU" << std::endl;
        }

        NasrSetTextureAsTarget( texture );
        NasrRect bg { 1.0f, 1.0f, w - 3.0f, h - 3.0f };
        NasrDrawGradientRectToTexture(
            bg,
            NASR_DIR_DOWN,
            { 224.0f, 0.0f, 0.0f, 255.0f },
            { 255.0f, 0.0f, 0.0f, 255.0f }
        );

        // Draw top side
        NasrDrawSpriteToTexture
        (
            menu_texture,
            { 6.0f, 0.0f, 1.0f, 5.0f },
            { 6.0f, 0.0f, w - 12.0f, 5.0f },
            0,
            0,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0,
            1
        );

        // Draw left side
        NasrDrawSpriteToTexture
        (
            menu_texture,
            { 0.0f, 6.0f, 5.0f, 1.0f },
            { 0.0f, 6.0f, 5.0f, h - 12.0f },
            0,
            0,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0,
            1
        );

        // Draw right side
        NasrDrawSpriteToTexture
        (
            menu_texture,
            { 8.0f, 6.0f, 5.0f, 1.0f },
            { w - 5.0f, 6.0f, 5.0f, h - 12.0f },
            0,
            0,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0,
            1
        );

        // Draw bottom side
        NasrDrawSpriteToTexture
        (
            menu_texture,
            { 6.0f, 8.0f, 1.0f, 5.0f },
            { 6.0f, h - 5.0f, w - 12.0f, 5.0f },
            0,
            0,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0,
            1
        );

        // Draw top-left corner
        NasrDrawSpriteToTexture
        (
            menu_texture,
            { 0.0f, 0.0f, 6.0f, 6.0f },
            { 0.0f, 0.0f, 6.0f, 6.0f },
            0,
            0,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0,
            1
        );

        // Draw top-right corner
        NasrDrawSpriteToTexture
        (
            menu_texture,
            { 7.0f, 0.0f, 6.0f, 6.0f },
            { w - 6.0f, 0.0f, 6.0f, 6.0f },
            0,
            0,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0,
            1
        );

        // Draw bottom-left corner
        NasrDrawSpriteToTexture
        (
            menu_texture,
            { 0.0f, 7.0f, 6.0f, 6.0f },
            { 0.0f, h - 6.0f, 6.0f, 6.0f },
            0,
            0,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0,
            1
        );

        // Draw bottom-right corner
        NasrDrawSpriteToTexture
        (
            menu_texture,
            { 7.0f, 7.0f, 6.0f, 6.0f },
            { w - 6.0f, h - 6.0f, 6.0f, 6.0f },
            0,
            0,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0,
            1
        );
        NasrReleaseTextureTarget();

        uint_fast8_t abs = GetArgConvert<uint_fast8_t, bool> ( "abs", args, true );
        unsigned int layer = GetArgConvert<unsigned int, Layer> ( "layer", args, Layer::AFTER_FG_2 );
        float opacity = GetArg( "opacity", args, 1.0f );
        float x = GetArg( "x", args, 0.0f );
        float y = GetArg( "y", args, 0.0f );

        int graphic = NasrGraphicsAddSprite
        (
            abs,
            current_state_,
            static_cast<unsigned int>( layer ),
            texture,
            { 0.0f, 0.0f, w, h },
            {
                align == Text::Align::CENTER ? ( ( static_cast<float> ( WINDOW_WIDTH_PIXELS ) - w ) / 2.0f ) : x,
                valign == Text::Valign::MIDDLE ? ( ( static_cast<float> ( WINDOW_HEIGHT_PIXELS ) - h ) / 2.0f ) : y,
                w,
                h
            },
            0,
            0,
            0.0f,
            0.0f,
            0.0f,
            opacity,
            0,
            1
        );

        if ( graphic < 0 )
        {
            // TODO: throw exception.
            std::cout << "NO MENU SPRITE" << std::endl;
        }

        return { static_cast<unsigned int>( graphic ) };
    };

    CounterGraphic Renderer::addCounter
    (
        float num,
        unsigned int maxdigits,
        uint_fast8_t palette,
        uint_fast8_t color,
        float x,
        float y,
        ArgList args
    ) const
    {
        uint_fast8_t abs = GetArgConvert<uint_fast8_t, bool> ( "abs", args, true );
        unsigned int layer = GetArgConvert<unsigned int, Layer> ( "layer", args, Layer::AFTER_FG_2 );
        float shadow = GetArg<float> ( "shadow", args, 0.0f );
        float opacity = GetArg( "opacity", args, 1.0f );
        unsigned int maxdecimals = GetArg( "maxdecimals", args, 0 );
        uint_fast8_t numpadding = GetArg( "numpadding", args, 0 );
        uint_fast8_t decimalpadding = GetArg( "decimalpadding", args, 0 );

        int graphic = NasrGraphicsAddCounterPalette
        (
            abs,
            current_state_,
            layer,
            charset_,
            num,
            maxdigits,
            maxdecimals,
            numpadding,
            decimalpadding,
            palette,
            color,
            1,
            x,
            y,
            shadow,
            opacity
        );

        if ( graphic < 0 )
        {
            // TODO: throw exception.
            std::cout << "NO COUNT" << std::endl;
        }

        return { static_cast<unsigned int>( graphic ) };
    };

    CounterGraphic Renderer::addCounterGradient
    (
        float num,
        unsigned int maxdigits,
        Dir::XY dir,
        uint_fast8_t color1,
        uint_fast8_t color2,
        float x,
        float y,
        ArgList args
    ) const
    {
        uint_fast8_t abs = GetArgConvert<uint_fast8_t, bool> ( "abs", args, true );
        unsigned int layer = GetArgConvert<unsigned int, Layer> ( "layer", args, Layer::AFTER_FG_2 );
        float shadow = GetArg<float> ( "shadow", args, 0.0f );
        float opacity = GetArg( "opacity", args, 1.0f );
        unsigned int maxdecimals = GetArg( "maxdecimals", args, 0 );
        uint_fast8_t numpadding = GetArg( "numpadding", args, 0 );
        uint_fast8_t decimalpadding = GetArg( "decimalpadding", args, 0 );

        int graphic = NasrGraphicsAddCounterPaletteGradient
        (
            abs,
            current_state_,
            layer,
            charset_,
            num,
            maxdigits,
            maxdecimals,
            numpadding,
            decimalpadding,
            0,
            static_cast<uint_fast8_t>( dir ),
            color1,
            color2,
            1,
            x,
            y,
            shadow,
            opacity
        );

        if ( graphic < 0 )
        {
            // TODO: throw exception.
            std::cout << "NO COUNT" << std::endl;
        }

        return { static_cast<unsigned int>( graphic ) };     
    };
}