#include "pi_block_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

#include <cstdio>

static constexpr int FALL_TIMER_LIMIT = 16;

PiBlockSprite::PiBlockSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/pi-block.png" ), x, y, 16, 16, { SpriteType::PI_BLOCK }, 200, 1000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false ),
	fall_timer_ ( 0 ),
	hero_still_on_block_ ( false )
{};

PiBlockSprite::~PiBlockSprite() {};

void PiBlockSprite::customUpdate( LevelState& level_state )
{
	if ( hero_still_on_block_ )
	{
		if ( fall_timer_ >= FALL_TIMER_LIMIT )
		{
			moveDown();
		}
		else
		{
			if ( fall_timer_ % 4 == 1 )
			{
				hit_box_.x = original_hit_box_.x - 1;
			}
			else if ( fall_timer_ % 4 == 3 )
			{
				hit_box_.x = original_hit_box_.x + 1;
			}
			else
			{
				hit_box_.x = original_hit_box_.x;
			}
			++fall_timer_;
		}
	}

	hero_still_on_block_ = false;
};

void PiBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( !them.hasType( SpriteType::PI_BLOCK ) && their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
		if ( them.hasType( SpriteType::HERO ) )
		{
			hero_still_on_block_ = true;
		}
	}
};

void PiBlockSprite::reset()
{
	hit_box_.x = original_hit_box_.x;
	hit_box_.y = original_hit_box_.y;
	vy_ = 0;
	acceleration_y_ = 0;
	fall_timer_ = 0;
};
