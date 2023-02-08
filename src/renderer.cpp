#include "nasringine/nasr.h"
#include "renderer.hpp"

#include <iostream>

namespace BSL
{
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

        unsigned int abs = 0;
        unsigned int layer = static_cast<int>( Layer::SPRITES_1 );
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

        return graphic;
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

        return graphic;
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

        return graphic;
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

        return graphic;
    };
}