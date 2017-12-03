


// Name
//===================================
//
// PenguinSprite
//


// DEPENDENCIES
//===================================

    #include "collision.hpp"
    #include "penguin_graphics.hpp"
    #include "penguin_sprite.hpp"
    #include "sprite_graphics.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    PenguinSprite::PenguinSprite( int x, int y )
    :
        Sprite( std::make_unique<PenguinGraphics> (), x, y, 16, 24, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 1200, 4000, 0, 0, Direction::Horizontal::LEFT ),
        turning_ ( false ),
        delay_ ( 48, false )
    {};

    PenguinSprite::~PenguinSprite() {};

    void PenguinSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
    {
        if ( direction_x_ == Direction::Horizontal::LEFT )
        {
            if ( !turning_ )
            {
                moveLeft();

                if ( hit_box_.x < original_hit_box_.x - Unit::BlocksToSubPixels( 3 ) )
                {
                    direction_x_ = Direction::Horizontal::RIGHT;
                    turning_ = true;
                }
            }

        }
        else if ( direction_x_ == Direction::Horizontal::RIGHT )
        {
            if ( !turning_ )
            {
                moveRight();

                if ( hit_box_.x > original_hit_box_.x )
                {
                    direction_x_ = Direction::Horizontal::LEFT;
                    turning_ = true;
                }
            }
        }

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

    };

    void PenguinSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
    {
    };
