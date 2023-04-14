#ifndef SPRITE_SYSTEM_H
#define SPRITE_SYSTEM_H

#include "sprite.hpp"
#include <vector>

namespace BSL
{
    class Controller;
    class Game;
    class Level;
    class Map;

    class SpriteSystem
    {
        public:
            SpriteSystem();
            void init( Game & game, Map & map );
            void update( float dt, const Controller & controller, Level & level, Game & game );
            inline Sprite & getHero() { return autumn_; };

        private:
            Sprite autumn_;
            std::vector<Sprite> sprites_;
    };
}

#endif // SPRITE_SYSTEM_H