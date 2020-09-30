#include "dragonfly_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

static constexpr int MOVE_DISTANCE = Unit::BlocksToSubPixels( 6 );

DragonflySprite::DragonflySprite( int x, int y, bool vertical )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/dragonfly.png", 0, 0, false, false, 0.0, -2, -14, 9, 16 ), x, y, 28, 8, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 1500, 1500, 0, 0, ( vertical ) ? Direction::Horizontal::__NULL : Direction::Horizontal::RIGHT, ( vertical ) ? Direction::Vertical::UP : Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    animation_timer_ ( 0 )
{};

DragonflySprite::~DragonflySprite() {};

void DragonflySprite::customUpdate( LevelState& level_state )
{
    switch ( direction_x_ )
    {
        case ( Direction::Horizontal::RIGHT ):
        {
            moveRight();
            if ( rightSubPixels() >= original_hit_box_.right() + MOVE_DISTANCE )
            {
                direction_x_ = Direction::Horizontal::LEFT;
            }
        }
        break;
        case ( Direction::Horizontal::LEFT ):
        {
            moveLeft();
            if ( hit_box_.x <= original_hit_box_.x )
            {
                direction_x_ = Direction::Horizontal::RIGHT;
            }
        }
        break;
        default:
        {
            switch ( direction_y_ )
            {
                case ( Direction::Vertical::UP ):
                {
                    moveUp();
                    if ( hit_box_.y <= original_hit_box_.y - MOVE_DISTANCE )
                    {
                        direction_y_ = Direction::Vertical::DOWN;
                    }
                }
                break;
                case ( Direction::Vertical::DOWN ):
                {
                    moveDown();
                    if ( bottomSubPixels() >= original_hit_box_.bottom() )
                    {
                        direction_y_ = Direction::Vertical::UP;
                    }
                }
                break;
                default:
                {
                }
                break;
            }
        }
        break;
    }

    ++animation_timer_;
    if ( animation_timer_ == 8 )
    {
        animation_timer_ = 0;
        graphics_->current_frame_x_ += 37;
        if ( graphics_->current_frame_x_ == 111 )
        {
            graphics_->current_frame_x_ = 0;
        }
    }
};

void DragonflySprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};
