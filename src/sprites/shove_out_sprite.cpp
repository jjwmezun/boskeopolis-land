#include "shove_out_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

static constexpr int MOVEMENT_SPEED = 250;

ShoveOutSprite::ShoveOutSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/shove-block.png", 0, 18 ), x + 8, y - 8, 32, 32, {}, 0, 0, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    status_ ( Status::WAITING_BEFORE_GOING_OUT ),
    timer_ ( 0 ),
    image_box_ ( x + 8, y - 8, 32, 32 )
{};

ShoveOutSprite::~ShoveOutSprite() {};

void ShoveOutSprite::customUpdate( LevelState& level_state )
{
    switch ( status_ )
    {
        case ( Status::WAITING_BEFORE_GOING_OUT ):
        {
            if ( timer_ <= 30 )
            {
                timer_ = 0;
                status_ = Status::GOING_OUT;
            }
            else
            {
                ++timer_;
            }
        }
        break;
        case ( Status::GOING_OUT ):
        {
            const int bottom_limit = original_hit_box_.y + Unit::PixelsToSubPixels( 18 );
            if ( hit_box_.y >= bottom_limit )
            {
                hit_box_.y = bottom_limit;
                status_ = Status::WAITING_BEFORE_GOING_IN;
            }
            else
            {
                hit_box_.y += MOVEMENT_SPEED;
                hit_box_.x -= MOVEMENT_SPEED;
            }
        }
        break;
        case ( Status::WAITING_BEFORE_GOING_IN ):
        {
            if ( timer_ <= 30 )
            {
                timer_ = 0;
                status_ = Status::GOING_IN;
            }
            else
            {
                ++timer_;
            }
        }
        break;
        case ( Status::GOING_IN ):
        {
            if ( hit_box_.y <= original_hit_box_.y )
            {
                hit_box_.y = original_hit_box_.y;
                status_ = Status::WAITING_BEFORE_GOING_OUT;
            }
            else
            {
                hit_box_.y -= MOVEMENT_SPEED;
                hit_box_.x += MOVEMENT_SPEED;
            }
        }
        break;
    }
    image_box_.h = Unit::SubPixelsToPixels( hit_box_.h + hit_box_.y - original_hit_box_.y );
    image_box_.w = Unit::SubPixelsToPixels( hit_box_.w + original_hit_box_.x - hit_box_.x );
    graphics_->current_frame_y_ = 50 - image_box_.h;
    image_box_.x = Unit::SubPixelsToPixels( hit_box_.x );
};

void ShoveOutSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::HERO ) )
    {
        if ( hit_box_.y >= original_hit_box_.y + Unit::PixelsToSubPixels( 8 ) )
        {
            Collision collision = them.testBlockCollision( *this );
            if ( collision.collideAny() )
            {
                if ( collision.collideBottom() )
                {
                    switch ( status_ )
                    {
                        case ( Status::GOING_OUT ):
                        {
                            them.hit_box_.x -= MOVEMENT_SPEED;
                        }
                        break;
                        case ( Status::GOING_IN ):
                        {
                            them.hit_box_.x += MOVEMENT_SPEED;
                        }
                        break;
                    }
                    them.collideStopAny( collision );
                }
                else if ( status_ == Status::GOING_OUT && them.rightSubPixels() > hit_box_.x + 4000 && them.hit_box_.x < rightSubPixels() - 4000 )
                {
                    them.block_interact_ = false;
                    them.on_ground_ = true;
                    them.hit_box_.x -= MOVEMENT_SPEED;
                    them.hit_box_.y = hit_box_.y + hit_box_.h - them.hit_box_.h;
                }
                else if ( !collision.collideTop() )
                {
                    them.collideStopAny( collision );
                }
            }
        }
        else
        {
            them.block_interact_ = true;
        }
    }
};

void ShoveOutSprite::render( const Camera& camera ) const
{
    graphics_->render( image_box_, &camera );
};
