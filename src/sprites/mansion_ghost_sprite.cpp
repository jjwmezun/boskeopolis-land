#include "audio.hpp"
#include "inventory_level.hpp"
#include "level_state.hpp"
#include "main.hpp"
#include "mansion_ghost_sprite.hpp"
#include "sprite_graphics.hpp"

static constexpr int LIGHT_LIMIT = 32;
static constexpr int ANIMATION_FRAMES[ MansionGhostSprite::NUMBER_OF_ANIMATION_FRAMES ] =
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

int MansionGhostSprite::last_ghost_death_frame_ = -1;

MansionGhostSprite::MansionGhostSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/kappa-obake-3.png", 0, 0, false, false, 0.0, true, -10, -6, 16, 7, 160, SDL_BLENDMODE_ADD ), x, y, 15, 18, { SpriteType::ENEMY, SpriteType::MANSION_GHOST, SpriteType::DEATH_COUNT }, 100, 1000, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true, false ),
	animation_counter_ (),
	light_timer_ ( 0 ),
	vertical_acceleration_ ( 0 ),
	vertical_speed_ ( 0 ),
	being_flashed_ ( false )
{};

MansionGhostSprite::~MansionGhostSprite() {};

void MansionGhostSprite::customUpdate( LevelState& level_state )
{
	being_flashed_ = collide_left_;
	if ( being_flashed_ )
	{
		++light_timer_;
		if ( !is_dead_ && light_timer_ > LIGHT_LIMIT )
		{
			kill();
			level_state.inventory().addGhostKill();
			last_ghost_death_frame_ = Main::stateFrame();
			Audio::playSound( Audio::SoundType::GHOST );
		}
		vertical_acceleration_ /= 1.15;
	}
	else
	{
		--light_timer_;
		if ( light_timer_ < 0 )
		{
			light_timer_ = 0;
		}
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
	}
	vertical_speed_ += vertical_acceleration_;
	hit_box_.y += vertical_speed_;

	if ( wave_timer_.update() )
	{
		direction_y_ = Direction::switchVertical( direction_y_ );
	}

	if ( being_flashed_ )
	{
		graphics_->current_frame_x_ = 155;
	}
	else
	{
		animation_counter_.update();
		graphics_->current_frame_x_ = ANIMATION_FRAMES[ animation_counter_.get() ];
	}
	flipGraphicsOnRight();
};

void MansionGhostSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( !being_flashed_ )
		{
			moveToward( them );
		}
		else
		{
			stopX();
			stopY();
			vertical_acceleration_ = 0;
		}
	}
};

void MansionGhostSprite::deathAction( LevelState& level_state )
{
	if ( graphics_->alpha_ == 0 )
	{
		death_finished_ = true;
	}
	else
	{
		if ( graphics_->alpha_ >= 4 )
		{
			graphics_->alpha_ -= 4;
		}
		else
		{
			graphics_->alpha_ = 0;
		}
		
	}
};