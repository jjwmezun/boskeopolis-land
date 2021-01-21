#include "bullet_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "shooter_guard_sprite.hpp"
#include "sprite_graphics.hpp"

ShooterGuardSprite::ShooterGuardSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 16, 24, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 1000, 1000, 0, 0, Direction::Horizontal::RIGHT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    state_ ( State::PACING ),
    timer_ ( 30 )
{};

ShooterGuardSprite::~ShooterGuardSprite() {};

void ShooterGuardSprite::customUpdate( LevelState& level_state )
{
    switch ( state_ )
    {
        case ( State::PACING ):
        {
            switch ( direction_x_ )
            {
                case ( Direction::Horizontal::LEFT ):
                {
                    moveLeft();
                    if ( hit_box_.x < original_hit_box_.x )
                    {
                        direction_x_ = Direction::Horizontal::RIGHT;
                    }
                }
                break;
                case ( Direction::Horizontal::RIGHT ):
                {
                    moveRight();
                    if ( rightSubPixels() > original_hit_box_.x + Unit::BlocksToSubPixels( 8 ) )
                    {
                        direction_x_ = Direction::Horizontal::LEFT;
                    }
                }
                break;
            }
        }
        break;
        case ( State::STOP ):
        {
            fullStopX();
            if ( timer_ == 30 )
            {
                level_state.sprites().spawn( std::make_unique<BulletSprite> ( centerXPixels(), yPixels(), Direction::horizontalToSimple( direction_x_ ), false ) );
                timer_ = 0;
            }
            else
            {
                ++timer_;
            }
        }
        break;
    }
};

void ShooterGuardSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::HERO ) )
    {
        state_ = State::PACING;
        switch ( direction_x_ )
        {
            case ( Direction::Horizontal::LEFT ):
            {
                const sdl2::SDLRect vision = { them.rightSubPixels(), hit_box_.y, hit_box_.x - them.rightSubPixels(), hit_box_.h };
                if
                (
                    them.rightSubPixels() > hit_box_.x - Unit::BlocksToSubPixels( 6 ) &&
                    them.hit_box_.x < hit_box_.x &&
                    them.hit_box_.y < bottomSubPixels() &&
                    them.bottomSubPixels() > hit_box_.y &&
                    !level_state.blocks().blocksInTheWay( vision, BlockComponent::Type::SOLID )
                )
                {
                    for ( const auto& sprite : level_state.sprites().getSpritesList() )
                    {
                        if
                        (
                            sprite &&
                            sprite->bottomSubPixels() > hit_box_.y &&
                            sprite->hit_box_.y < bottomSubPixels() &&
                            sprite->rightSubPixels() >= them.rightSubPixels() &&
                            sprite->hit_box_.x <= hit_box_.x
                        )
                        {
                            return;
                        }
                    }
                    state_ = State::STOP;
                }
            }
            break;
            case ( Direction::Horizontal::RIGHT ):
            {
                if
                (
                    them.hit_box_.x < rightSubPixels() + Unit::BlocksToSubPixels( 6 ) &&
                    them.rightSubPixels() > rightSubPixels() &&
                    them.hit_box_.y < bottomSubPixels() &&
                    them.bottomSubPixels() > hit_box_.y &&
                    !level_state.blocks().blocksInTheWay({ rightSubPixels(), hit_box_.y, them.hit_box_.x - rightSubPixels(), hit_box_.h }, BlockComponent::Type::SOLID )
                )
                {
                    for ( const auto& sprite : level_state.sprites().getSpritesList() )
                    {
                        if
                        (
                            sprite &&
                            sprite->bottomSubPixels() > hit_box_.y &&
                            sprite->hit_box_.y < bottomSubPixels() &&
                            sprite->rightSubPixels() <= them.hit_box_.x &&
                            sprite->hit_box_.x >= rightSubPixels()
                        )
                        {
                            return;
                        }
                    }
                    state_ = State::STOP;
                }
            }
            break;
        }
    }
};
