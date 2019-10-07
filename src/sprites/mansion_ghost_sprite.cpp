#include "mansion_ghost_sprite.hpp"
#include "sprite_graphics.hpp"

static constexpr int LIGHT_LIMIT = 32;
static constexpr int WAVE_TIMER_LIMIT = 32;

MansionGhostSprite::MansionGhostSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/ghost.png", 0, 0, false, false, 0.0, false, -4, -4, 8, 8, 160, SDL_BLENDMODE_ADD ), x, y, 8, 8, { SpriteType::ENEMY, SpriteType::MANSION_GHOST }, 100, 1000, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true, false ),
	light_timer_ ( 0 ),
	being_flashed_ ( false ),
	vertical_acceleration_ ( 0 ),
	vertical_speed_ ( 0 ),
	wave_timer_ ( 0 )
{};

MansionGhostSprite::~MansionGhostSprite() {};

void MansionGhostSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	being_flashed_ = collide_left_;
	if ( being_flashed_ )
	{
		++light_timer_;
		if ( light_timer_ > LIGHT_LIMIT )
		{
			kill();
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
		if ( vertical_speed_ > 500 )
		{
			vertical_speed_ = 500;
		}
		else if ( vertical_speed_ > -500 )
		{
			vertical_speed_ = -500;
		}
	}
	vertical_speed_ += vertical_acceleration_;
	hit_box_.y += vertical_speed_;

	if ( wave_timer_ == WAVE_TIMER_LIMIT )
	{
		wave_timer_ = 0;
		direction_y_ = Direction::switchVertical( direction_y_ );
	}
	else
	{
		++wave_timer_;
	}

	graphics_->current_frame_x_ = ( being_flashed_ ) ? 16 : 0;
	flipGraphicsOnRight();
};

void MansionGhostSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( !being_flashed_ )
		{
			moveToward( them );
			if ( direction_y_ == Direction::Vertical::DOWN )
			{
				vertical_acceleration_ = 50;
			}
			else
			{
				vertical_acceleration_ = -50;
			}
			
		}
		else
		{
			stopX();
			stopY();
			vertical_acceleration_ = 0;
		}
	}
};
