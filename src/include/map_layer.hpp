#ifndef MAP_LAYER_H
#define MAP_LAYER_H

namespace BSL
{
    class Game;
    class Level;

    class MapLayer
    {
        public:
            virtual void update( Level & level, const Game & game, float dt ) = 0;
            virtual void init( const Game & game ) = 0;
    };
}

#endif // MAP_LAYER_H