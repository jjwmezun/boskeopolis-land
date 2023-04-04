#include "sprite_system.hpp"

namespace BSL
{
    void SpriteSystem::init( Game & game )
    {
        autumn_.init( game );
    };

    void SpriteSystem::update( float dt, const Controller & controller, Level & level, Game & game )
    {
        autumn_.update( dt, controller, level, game );
    };
}