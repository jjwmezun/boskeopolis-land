#include "block_component.hpp"
#include "block_system.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "guard_sprite.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

static const int DISTANCE_LIMIT_X = 8 * 16000 - 4000;
static const int DISTANCE_LIMIT_Y = 16000;

GuardSprite::GuardSprite( int x, int y )
:
    Sprite( std::make_unique<SpriteGraphics> ( "sprites/guard.png", 0, 0, false, false, 0, 0, 0, 0, 0 ), x, y, 16, 24, { SpriteType::ENEMY }, 400, 1200, 0, 0, Direction::Horizontal::LEFT ),
    stunned_ ( false ),
    distance_limit_y_ ( DISTANCE_LIMIT_Y ),
    animation_frame_ (),
    stun_timer_ (),
    animation_timer_ ()
{};
GuardSprite::~GuardSprite() {};

void GuardSprite::customUpdate( LevelState& level_state )
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
    updateGraphics();
};

void GuardSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
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
        BlockSystem& blocks = level_state.blocks();
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

void GuardSprite::updateGraphics()
{
	flipGraphicsOnRight();
	if ( animation_timer_.hit() )
	{
		++animation_frame_;
        graphics_->current_frame_x_ = ( animation_frame_() ) ? 16 : 0;
	}
	animation_timer_.update();
}
