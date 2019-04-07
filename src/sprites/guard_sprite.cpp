#include "block_component.hpp"
#include "block_system.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "guard_graphics.hpp"
#include "guard_sprite.hpp"
#include "sprite_graphics.hpp"

static const int DISTANCE_LIMIT_X = 8 * 16000 - 4000;
static const int DISTANCE_LIMIT_Y = 16000;

GuardSprite::GuardSprite( int x, int y )
:
    Sprite( std::make_unique<GuardGraphics> (), x, y, 16, 24, { SpriteType::ENEMY }, 400, 1200, 0, 0, Direction::Horizontal::LEFT ),
    stunned_ ( false ),
    distance_limit_y_ ( DISTANCE_LIMIT_Y ),
    stun_timer_ ()
{};
GuardSprite::~GuardSprite() {};

void GuardSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
    moveInDirectionX();

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
        {
            stunned_ = false;
        }
    }
};

void GuardSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
    if ( them.hasType( SpriteType::HERO ) && !stunned_ )
    {
        if ( their_collision.collideAny() )
        {
            them.kill();
        }

        // Guards can notice higher up if hero is jumping 'bove them.
        const bool is_hero_jumping_above_them = !them.onGround() && them.bottomSubPixels() < centerYSubPixels();
        distance_limit_y_ = ( is_hero_jumping_above_them )
            ? DISTANCE_LIMIT_Y * 2
            : DISTANCE_LIMIT_Y;

        // If guard can see hero, die.
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
