


// Name
//===================================
//
// PenguinSprite
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "penguin_graphics.h"
    #include "penguin_sprite.h"
    #include "sprite_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    PenguinSprite::PenguinSprite( int x, int y )
    :
        Sprite( std::unique_ptr<SpriteGraphics> ( new PenguinGraphics() ), x, y, 16, 24, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 1200, 4000, 0, 0, Direction::Horizontal::LEFT ),
        turning_ ( false ),
        delay_ ( 48, false )
    {};

    PenguinSprite::~PenguinSprite() {};

    void PenguinSprite::customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites )
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

    void PenguinSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites )
    {
    };
