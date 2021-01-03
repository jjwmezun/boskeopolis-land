#include "audio.hpp"
#include "event_system.hpp"
#include "inventory_level.hpp"
#include "level_state.hpp"
#include "main.hpp"
#include "nocturnal_ghost_sprite.hpp"
#include "sprite_graphics.hpp"

static constexpr int LIGHT_LIMIT = 32;
static constexpr int ANIMATION_FRAMES[ NocturnalGhostSprite::NUMBER_OF_ANIMATION_FRAMES ] =
{
	0,
	31,
	62,
	93,
	93,
	62,
	31,
	0
};

NocturnalGhostSprite::NocturnalGhostSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/kappa-obake-3.png", 0, 0, false, false, 0.0, -10, -6, 16, 7, 160, SDL_BLENDMODE_ADD ), x, y, 15, 18, { SpriteType::ENEMY }, 100, 1000, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false ),
	animation_counter_ (),
	vertical_acceleration_ ( 0 ),
	vertical_speed_ ( 0 ),
    first_ ( true ),
    hit_ ( false )
{
	layer_ = Unit::Layer::SPRITES_2;
};

NocturnalGhostSprite::~NocturnalGhostSprite() {};

void NocturnalGhostSprite::customUpdate( LevelState& level_state )
{
    const bool prev_hit_ = hit_;
    hit_ = level_state.events().isSwitchOn();
    sprite_interact_ = !hit_;

    if ( first_ )
    {
        first_ = false;
        if ( hit_ )
        {
            sprite_interact_ = false;
            graphics_->alpha_ = 0;
            stopX();
            stopY();
            vertical_acceleration_ = 0;
            return;
        }
    }

    if ( hit_ )
    {
        if ( !prev_hit_ )
        {
            Audio::playSound( Audio::SoundType::GHOST );
        }
        stopX();
        stopY();
        vertical_acceleration_ = 0;

		graphics_->current_frame_x_ = 155;
        if ( graphics_->alpha_ >= 4 )
        {
            graphics_->alpha_ -= 4;
        }
        else
        {
            graphics_->alpha_ = 0;
        }
    }
    else
    {
        if ( direction_y_ == Direction::Vertical::DOWN )
        {
            vertical_acceleration_ = 50;
        }
        else
        {
            vertical_acceleration_ = -50;
        }
        if ( vertical_speed_ > 500 )
        {
            vertical_speed_ = 500;
        }
        else if ( vertical_speed_ < -500 )
        {
            vertical_speed_ = -500;
        }
        vertical_speed_ += vertical_acceleration_;
        hit_box_.y += vertical_speed_;

        if ( wave_timer_.update() )
        {
            direction_y_ = Direction::switchVertical( direction_y_ );
        }

		animation_counter_.update();
		graphics_->current_frame_x_ = ANIMATION_FRAMES[ animation_counter_.get() ];
	    flipGraphicsOnRight();
        if ( graphics_->alpha_ == 0 )
        {
            hit_box_ = original_hit_box_;
        }
        if ( graphics_->alpha_ <= 251 )
        {
            graphics_->alpha_ += 4;
        }
        else
        {
            graphics_->alpha_ = 255;
        }
    }
};

void NocturnalGhostSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( !hit_ && them.hasType( SpriteType::HERO ) )
	{
        moveToward( them );
	}
};

void NocturnalGhostSprite::deathAction( LevelState& level_state )
{

};

void NocturnalGhostSprite::reset()
{
};