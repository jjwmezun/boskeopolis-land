#include "cuckoo_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

CuckooSprite::CuckooSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 32, 32, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 500, 6000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false ),
    status_ ( Status::DORMANT ),
    timer_ ( 0 )
{};

CuckooSprite::~CuckooSprite() {};

void CuckooSprite::customUpdate( LevelState& level_state )
{
    switch ( status_ )
    {
        case ( Status::POPPING_OUT ):
        {
            if ( hit_box_.x <= original_hit_box_.x - Unit::BlocksToSubPixels( 4 ) )
            //if ( vx_ >= top_speed_ )
            {
                stopX();
                status_ = Status::OUT;
            }
            else
            {
                moveLeft();
            }
        }
        break;
        case ( Status::OUT ):
        {
            if ( vx_ <= 1 )
            {
                if ( timer_ == 20 )
                {
                    timer_ = 0;
                    status_ = Status::REELING_IN;
                }
                else
                {
                    ++timer_;
                }
            }
            else
            {
                vx_ /= 1.15;
            }
        }
        break;
        case ( Status::REELING_IN ):
        {
            if ( hit_box_.x >= original_hit_box_.x )
            {
                fullStopX();
                hit_box_.x = original_hit_box_.x;
                status_ = Status::DORMANT;
            }
            else
            {
                moveRight();
            }
        }
        break;
        case ( Status::RESTING ):
        {
            if ( timer_ == 20 )
            {
                timer_ = 0;
                status_ = Status::DORMANT;
            }
            else
            {
                ++timer_;
            }
        }
        break;
    }
};

void CuckooSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( status_ == Status::DORMANT )
    {
        if ( them.hasType( SpriteType::HERO ) )
        {
            if ( them.hit_box_.y <= bottomSubPixels() && them.bottomSubPixels() >= hit_box_.y && them.hit_box_.x < hit_box_.x && them.rightSubPixels() > hit_box_.x - Unit::BlocksToSubPixels( 4 ) )
            {
                status_ = Status::POPPING_OUT;
            }
        }
    }
};
