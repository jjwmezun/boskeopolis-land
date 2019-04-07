#include "collision.hpp"
#include "penguin_graphics.hpp"
#include "penguin_sprite.hpp"
#include "sprite_graphics.hpp"

PenguinSprite::PenguinSprite( int x, int y )
:
    Sprite( std::make_unique<PenguinGraphics> (), x, y, 16, 24, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 1200, 4000, 0, 0, Direction::Horizontal::LEFT ),
    turning_ ( false ),
    delay_ ()
{};

PenguinSprite::~PenguinSprite() {};

void PenguinSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
    if ( turning_ )
    {
        if ( delay_.done() )
        {
            turning_ = false;
            delay_.stop();
        }
        else if ( delay_.on() )
        {
            delay_.update();
        }
        else
        {
            delay_.start();
            acceleration_x_ = 0;
        }
    }
    else
    {
        switch( direction_x_ )
        {
            case( Direction::Horizontal::LEFT ):
            {
                moveLeft();
                if ( hit_box_.x < original_hit_box_.x - Unit::BlocksToSubPixels( 1 ) )
                {
                    direction_x_ = Direction::Horizontal::RIGHT;
                    turning_ = true;
                }
            }
            break;

            case( Direction::Horizontal::RIGHT ):
            {
                moveRight();
                if ( hit_box_.x > original_hit_box_.x )
                {
                    direction_x_ = Direction::Horizontal::LEFT;
                    turning_ = true;
                }
            }
            break;
        }
    }
};

void PenguinSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) {};
