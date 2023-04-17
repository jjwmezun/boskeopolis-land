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

        private:
            std::vector<Sprite> sprites_;

            Sprite createAutumnSprite();
            Sprite createCrabSprite( float x, float y );
            Sprite createBadAppleSprite( float x, float y, Dir::X dir );
            Sprite createGrumpTruckSprite( float x, float y, Dir::X dir );
    };
}

#endif // SPRITE_SYSTEM_H