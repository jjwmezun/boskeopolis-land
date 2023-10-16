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
            void init( Game & game, Map & map, float x, float y );
            void update( float dt, const Controller & controller, Level & level, Game & game );

        private:
            std::vector<Sprite> sprites_;

            Sprite createAutumnSprite( float x, float y );
            Sprite createCrabSprite( float x, float y );
            Sprite createBadAppleSprite( float x, float y, Dir::X dir );
            Sprite createGrumpTruckSprite( float x, float y, Dir::X dir );
            Sprite createPufferBeeSprite( float x, float y, Dir::X dir, SpriteComponent c1, SpriteComponent c2 );
    };
}

#endif // SPRITE_SYSTEM_H