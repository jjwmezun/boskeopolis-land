#include "bullet_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "shooter_guard_sprite.hpp"
#include "sprite_graphics.hpp"

ShooterGuardSprite::ShooterGuardSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/police.png", 0, 0, false, false, 0.0, -6, 0, 8, 0 ), x, y, 10, 24, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 1000, 1000, 0, 0, Direction::Horizontal::RIGHT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    state_ ( State::PACING ),
    shoot_timer_ ()
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
                    graphics_->flip_x_ = false;
                    moveLeft();
                    if ( hit_box_.x < original_hit_box_.x )
                    {
                        direction_x_ = Direction::Horizontal::RIGHT;
                    }
                }
                break;
                case ( Direction::Horizontal::RIGHT ):
                {
                    graphics_->flip_x_ = true;
                    moveRight();
                    if ( rightSubPixels() > original_hit_box_.x + Unit::BlocksToSubPixels( 8 ) )
                    {
                        direction_x_ = Direction::Horizontal::LEFT;
                    }
                }
                break;
            }
            if ( animation_timer_.update() )
            {
                graphics_->current_frame_x_ = ( graphics_->current_frame_x_ == 28 ) ? 46 : 28;
            }
        }
        break;
        case ( State::STOP ):
        {
            graphics_->current_frame_x_ = 28;
            animation_timer_.reset();
            fullStopX();
            if ( shoot_timer_.update() )
            {
                level_state.sprites().spawn( std::make_unique<BulletSprite> ( centerXPixels(), yPixels(), Direction::horizontalToSimple( direction_x_ ), false ) );
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

void ShooterGuardSprite::deathAction( LevelState& level_state )
{
    graphics_->x_adjustment_ = -2;
    graphics_->w_adjustment_ = 5;
    changeMovement( SpriteMovement::Type::FLOATING );
    graphics_->current_frame_x_ = 78;
	acceleration_x_ = 0;
	vx_ = 0;
	block_interact_ = false;
	sprite_interact_ = false;
    hit_box_.y += Unit::PixelsToSubPixels( 1 );
    switch ( direction_x_ )
    {
        case ( Direction::Horizontal::LEFT ):
        {
            graphics_->rotation_ -= 10.0;
            if ( graphics_->rotation_ <= -90.0 )
            {
                death_finished_ = true;
            }
        }
        break;
        case ( Direction::Horizontal::RIGHT ):
        {
            graphics_->rotation_ += 10.0;
            if ( graphics_->rotation_ >= 90.0 )
            {
                death_finished_ = true;
            }
        }
        break;
    }
};
