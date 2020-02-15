#pragma once

#include "mezun_sdl2.hpp"
#include "sprite_graphics.hpp"
#include <vector>

class Camera;

class LevelGraphics
{
    public:
        struct Sprite
        {
            SDL_Texture* texture_;
            sdl2::SDLRect source_;
            sdl2::SDLRect dest_;
            SDL_RendererFlip flip_;
            double rotation_;
            Uint8 alpha_;
            SDL_BlendMode blend_mode_;
        };
        enum class Type
        {
            SPRITE
        };
        enum class Tag
        {
            HERO
        };
        union Value
        {
            Sprite sprite_;
        };
        void render( const Camera* camera = nullptr ) const;

        Type type_;
        std::vector<Tag> tags_;
        Value value_;
};