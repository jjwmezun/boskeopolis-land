#ifndef RENDERER_H
#define RENDERER_H

#include "arg.hpp"
#include "dir.hpp"
#include "nasringine/nasr.h"
#include <vector>

namespace BSL
{
    class SpriteGraphic
    {
        public:
            constexpr SpriteGraphic( unsigned int gfx = 0 ) : gfx_ ( gfx ) {};
            inline void setOpacity( float v ) const
            {
                NasrGraphicsSpriteSetOpacity( gfx_, v );
            };

        private:
            unsigned int gfx_;
    };

    class TextGraphic
    {
        public:
            constexpr TextGraphic( unsigned int gfx = 0 ) : gfx_ ( gfx ) {};
            inline void setOpacity( float v ) const
            {
                NasrSetTextOpacity( gfx_, v );
            };

        private:
            unsigned int gfx_;
    };

    class CounterGraphic
    {
        public:
            constexpr CounterGraphic( unsigned int gfx = 0 ) : gfx_ ( gfx ) {};
            inline void setNumber( float v ) const
            {
                NasrGraphicsCounterSetNumber( gfx_, v );
            }

        private:
            unsigned int gfx_;
    };

    class Renderer
    {
        friend class Game;

        public:
            Renderer();

            unsigned int addSprite
            (
                std::string texture,
                float srcx,
                float srcy,
                float w,
                float h,
                float x,
                float y,
                ArgList args = {}
            ) const;

            unsigned int addRect
            (
                float x,
                float y,
                float w,
                float h,
                uint_fast8_t color,
                bool abs = false,
                Layer layer = Layer::BG_1
            ) const;

            unsigned int addRectGradient
            (
                float x,
                float y,
                float w,
                float h,
                Dir::XY dir,
                uint_fast8_t color1,
                uint_fast8_t color2,
                bool abs = false,
                Layer layer = Layer::BG_1
            ) const;

            unsigned int addTilemap
            (
                const std::string & tileset,
                const std::vector<NasrTile> & tiles,
                unsigned int width,
                unsigned height,
                ArgList args = {}
            ) const;

            TextGraphic addText
            (
                ArgList args
            ) const;

            CounterGraphic addCounter
            (
                ArgList args
            ) const;

            SpriteGraphic addMenuBox
            (
                Text::Align align,
                Text::Valign valign,
                float w,
                float h,
                ArgList args = {}
            ) const;

        private:
            unsigned int current_state_;
            unsigned int charset_;
    };
}

#endif // RENDERER_H