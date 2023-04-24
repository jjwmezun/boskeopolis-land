#ifndef SCALE_LIFT_SPRITE_H
#define SCALE_LIFT_SPRITE_H

#include "sprite.hpp"

namespace BSL
{
    class Game;

    Sprite createScaleLiftSprite( float x, float y, Dir::X dir );
    void initScaleLiftSprite( Game & game, Sprite & sprite );
    void updateScaleLiftSprite( float dt, Level & level, std::vector<Sprite> & sprites, Sprite & sprite );
    void collideDownScaleLiftSprite( Sprite & sprite, Sprite & other, float dt, Level & level, Game & game );
    void collideUpScaleLiftSprite( Sprite & sprite, Sprite & other, float dt, Level & level, Game & game );
    void collideXScaleLiftSprite( Sprite & sprite, Sprite & other, float dt, Level & level, Game & game );
};

#endif // SCALE_LIFT_SPRITE_H