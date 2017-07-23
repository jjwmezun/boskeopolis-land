




// Name
//===================================
//
// GuardSprite
//


// DEPENDENCIES
//===================================

    #include "block_component.hpp"
    #include "block_system.hpp"
    #include "camera.hpp"
    #include "collision.hpp"
    #include "guard_graphics.hpp"
    #include "guard_sprite.hpp"
    #include "sprite_graphics.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    GuardSprite::GuardSprite( int x, int y )
    :
        Sprite( std::make_unique<GuardGraphics> (), x, y, 16, 24, { SpriteType::ENEMY }, 400, 1200 )
    {
        direction_x_ = Direction::Horizontal::LEFT;
    };

    GuardSprite::~GuardSprite() {};

    void GuardSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {
        if ( direction_x_ == Direction::Horizontal::RIGHT )
        {
            moveRight();
        }
        else
        {
            moveLeft();
        }

        if ( collidedRight() )
        {
            direction_x_ = Direction::Horizontal::LEFT;
            stunned_ = true;
        }
        if ( collidedLeft() )
        {
            direction_x_ = Direction::Horizontal::RIGHT;
            stunned_ = true;
        }

        if ( stunned_ && !stun_timer_.on() )
        {
            stun_timer_.start();
        }
        else if ( stunned_ )
        {
            stun_timer_.update();

            if ( stun_timer_.done() )
                stunned_ = false;
        }
    };

    void GuardSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
    {
        if ( them.hasType( SpriteType::HERO ) && !stunned_ )
        {
            if ( !them.onGround() && them.bottomSubPixels() < centerYSubPixels() )
                distance_limit_y_ = DISTANCE_LIMIT_Y*2;
            else
                distance_limit_y_ = DISTANCE_LIMIT_Y;

            if ( their_collision.collideAny() )
                them.kill();

            if ( direction_x_ == Direction::Horizontal::RIGHT )
            {
                if
                (
                    them.leftSubPixels() > leftSubPixels() &&
                    them.leftSubPixels() < rightSubPixels() + DISTANCE_LIMIT_X &&
                    them.bottomSubPixels() > topSubPixels() - distance_limit_y_ &&
                    them.topSubPixels() < bottomSubPixels()
                )
                {
                    if
                    (
                        !blocks.blocksInTheWay
                        (
							{ rightSubPixels(),
                            them.topSubPixels(),
                            them.leftSubPixels() - rightSubPixels(),
                            them.heightSubPixels() },
                            BlockComponent::Type::SOLID
                        )
                    )
                    {
                        them.kill();
                    }
                }
            }
            else if ( direction_x_ == Direction::Horizontal::LEFT )
            {
                if
                (
                    them.rightSubPixels() < rightSubPixels() &&
                    them.rightSubPixels() > leftSubPixels() - DISTANCE_LIMIT_X &&
                    them.bottomSubPixels() > topSubPixels() - distance_limit_y_ &&
                    them.topSubPixels() < bottomSubPixels()
                )
                {
                    if
                    (
                        !blocks.blocksInTheWay
                        (
							{ them.rightSubPixels(),
                            them.topSubPixels(),
                            leftSubPixels() - them.rightSubPixels(),
                            them.heightSubPixels() },
                            BlockComponent::Type::SOLID
                        )
                    )
                    {
                        them.kill();
                    }
                }
            }
        }
    };
