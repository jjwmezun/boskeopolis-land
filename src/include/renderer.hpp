#ifndef RENDERER_H
#define RENDERER_H

#include "arg.hpp"
#include "dir.hpp"
#include "nasringine/nasr.h"
#include <vector>

namespace BSL
{
    class Renderer
    {
        friend class Game;

        public:
            enum class Layer
            {
                BEFORE_BG_1 = 0,
                BG_1 = 1,
                BEFORE_BG_2 = 2,
                BG_2 = 3,
                BEFORE_BLOCKS_1 = 4,
                BLOCKS_1 = 5,
                BEFORE_SPRITES_1 = 6,
                SPRITES_1 = 7,
                BEFORE_BLOCKS_2 = 8,
                BLOCKS_2 = 9,
                BEFORE_SPRITES_2 = 10,
                SPRITES_2 = 11,
                BEFORE_FG_1 = 12,
                FG_1 = 13,
                BEFORE_FG_2 = 14,
                FG_2 = 15,
                AFTER_FG_2 = 16,
                SUPER = 17
            };

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

        private:
            unsigned int current_state_;
    };
}

#endif // RENDERER_H