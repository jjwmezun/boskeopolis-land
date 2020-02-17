#include "falling_spike_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

FallingSpikeSprite::FallingSpikeSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/falling-spike.png" ), x, y, 16, 16, { SpriteType::ENEMY }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false ),
    state_ ( State::STATIC ),
    timer_ (),
    frame_ ( 0 )
{};

FallingSpikeSprite::~FallingSpikeSprite() {};

void FallingSpikeSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
    switch ( state_ )
    {
        case ( State::SHAKING ):
        {
            if ( timer_.update() )
            {
                ++frame_;
                hit_box_.x = original_hit_box_.x + Unit::PixelsToSubPixels( FRAMES[ frame_ ] );
                if ( frame_ == NUMBER_OF_FRAMES )
                {
                    state_ = State::FALLING;
                    changeMovement( SpriteMovement::Type::GROUNDED );
                }
            }
        }
        break;
    }
};

void FallingSpikeSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( state_ == State::STATIC && them.hasType( SpriteType::HERO ) )
	{
        if
        (
            them.hit_box_.x > hit_box_.x - Unit::BlocksToSubPixels( 2 ) &&
            them.rightSubPixels() < hit_box_.x + Unit::BlocksToSubPixels( 2 ) &&
            them.hit_box_.y > hit_box_.y &&
            them.bottomSubPixels() < bottomSubPixels() + Unit::BlocksToSubPixels( 4 )
        )
        {
            state_ = State::SHAKING;
        }
	}
};

void FallingSpikeSprite::render( Camera& camera, bool priority ) {};

void FallingSpikeSprite::renderSuperPriority( Camera& camera )
{
    graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera );
};

void FallingSpikeSprite::reset()
{
    hit_box_.y = original_hit_box_.y;
    state_ = State::STATIC;
    frame_ = 0;
    changeMovement( SpriteMovement::Type::FLOATING );
};