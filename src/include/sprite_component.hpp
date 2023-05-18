#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "arg.hpp"
#include <vector>

namespace BSL
{
    class Game;
    class Level;
    class Sprite;

    struct SpriteComponent
    {
        enum class Type
        {
            __NULL,
            MOVEX,
            MOVEY,
            CIRCLE
        };
        static SpriteComponent create( Type type, const ArgList & args = {} );
        void update
        (
            Sprite & sprite,
            float dt,
            Level & level,
            Game & game,
            std::vector<Sprite> & sprites
        );

        Type type_;
        union
        {
            struct
            {
                Dir::X dir;
                float width;
                float acc;
                float maxspeed;
            }
            movex;
            struct
            {
                Dir::Y dir;
                float height;
                float acc;
                float maxspeed;
            }
            movey;
            struct
            {
                float startx;
                float starty;
                float radius;
                float speed;
                float angle;
            }
            circle;
        }
        data_;
    };
}

#endif // SPRITE_COMPONENT_H