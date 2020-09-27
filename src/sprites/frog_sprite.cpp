#include "frog_sprite.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"

static constexpr int TONGUE_RELATIVE_Y = Unit::PixelsToSubPixels( 7 );

FrogSprite::FrogSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/frog.png", 0, 0, false, false, 0.0, false, -1, -8, 2, 9 ), x, y, 26, 20, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 250, 2000, 250, 2000, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr ),
    jump_timer_ (),
    tongue_ ( 0, 0, 0, Unit::PixelsToSubPixels( 4 ) ),
    state_ ( FrogState::STATIONARY )
{};

FrogSprite::~FrogSprite() {};

void FrogSprite::customUpdate( LevelState& level_state )
{
    switch ( state_ )
    {
        case ( FrogState::STATIONARY ):
        {
            if ( jump_timer_.update() )
            {
                //state_ = ( mezun::testRandomWithinPercentage( 33 ) ) ? FrogState::LICKING : FrogState::JUMPING;
                state_ = FrogState::JUMPING;
                if ( state_ == FrogState::JUMPING )
                {
                    jump_top_speed_ = ( mezun::testRandomWithinPercentage( 25 ) ) ? 4000 : 2000;
                    jump();
                    moveInDirectionX();
                }
                else
                {
                    setTongueY();
                }
            }
        }
        break;
        case ( FrogState::JUMPING ):
        {
            if ( on_ground_ )
            {
                fullStopX();
                state_ = FrogState::STATIONARY;
            }
            else
            {
                jump();
                moveInDirectionX();
            }
        }
        break;
        case ( FrogState::LICKING ):
        {
            tongue_speed_ = std::min( 2000, tongue_speed_ + 250 );
            tongue_.w += tongue_speed_;
            setTongueX();
            if ( tongue_.w >= Unit::BlocksToSubPixels( 4 ) )
            {
                state_ = FrogState::LICKING_BACK;
            }
        }
        break;
        case ( FrogState::LICKING_BACK ):
        {
            tongue_speed_ = std::min( 2000, tongue_speed_ + 250 );
            tongue_.w -= tongue_speed_;
            setTongueX();
            if ( tongue_.w <= 0 )
            {
                state_ = FrogState::STATIONARY;
            }
        }
        break;
    }
    flipGraphicsOnRight();
};

void FrogSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::HERO ) )
    {
        switch ( state_ )
        {
            case ( FrogState::STATIONARY ):
            {
                if ( isLeftOf( them ) )
                {
                    direction_x_ = Direction::Horizontal::RIGHT;
                }
                else if ( isRightOf( them ) )
                {
                    direction_x_ = Direction::Horizontal::LEFT;
                }
            }
            break;

            case ( FrogState::LICKING ):
            case ( FrogState::LICKING_BACK ):
            {
                const Collision tongue_collision = movement_->testCollision( them, tongue_ );
                if ( tongue_collision.collideAny() )
                {
                    level_state.health().hurt();
                }
            }
            break;
        }
    }
};

void FrogSprite::render( Camera& camera, bool priority )
{
    graphics_->current_frame_x_ = 0;
    graphics_->current_frame_y_ = 20;
    graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
    //graphics_->current_frame_y_ = 16;
    //graphics_->render( Unit::SubPixelsToPixels( tongue_ ), &camera, priority );
};

void FrogSprite::setTongueX()
{
    tongue_.x = ( direction_x_ == Direction::Horizontal::LEFT ) ? hit_box_.x - tongue_.w : rightSubPixels();
};

void FrogSprite::setTongueY()
{
    tongue_.y = hit_box_.y + TONGUE_RELATIVE_Y;
};

void FrogSprite::deathAction( LevelState& level_state )
{
    setTongueY();
	defaultDeathAction( level_state.camera() );
};