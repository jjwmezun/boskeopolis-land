#ifndef SPRITE_SYSTEM_H
#define SPRITE_SYSTEM_H

#include "sprite.hpp"

namespace BSL
{
    class Controller;
    class Game;
    class Level;

    class SpriteSystem
    {
        public:
            void init( Game & game );
            void update( float dt, const Controller & controller, Level & level, Game & game );
            inline Sprite & getHero() { return autumn_; };

        private:
            Sprite autumn_;
    };
}

#endif // SPRITE_SYSTEM_H