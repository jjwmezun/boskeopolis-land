#include "audio.hpp"
#include "target_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

TargetSprite::TargetSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 8, 8, {}, 2000, 2000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true, true  )
{};

TargetSprite::~TargetSprite() {};

void TargetSprite::customUpdate( LevelState& level_state )
{
    switch ( direction_y_ )
    {
        case ( Direction::Vertical::UP ):
        {
            moveUp();
            if ( collide_bottom_ )
            {
                direction_y_ = Direction::Vertical::DOWN;
            }
        }
        break;
        case ( Direction::Vertical::DOWN ):
        {
            moveDown();
            if ( collide_top_ )
            {
                direction_y_ = Direction::Vertical::UP;
            }
        }
        break;
    }
};

void TargetSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::HEROS_BULLET ) && their_collision.collideAny() )
    {
        killNoAnimation();
        them.kill();
        Audio::playSound( Audio::SoundType::BOP );
    }
};
