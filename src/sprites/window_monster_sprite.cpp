#include "audio.hpp"
#include "window_monster_sprite.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

static constexpr int FRAME_SIZE = 38;
static constexpr int ROTATION_SPEED = 2;

WindowMonsterSprite::WindowMonsterSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/window-monster.png", 0, 0, false, false, 0, false, -5, -5, 10, 10 ), x, y - 8, 26, 26, {}, 500, 2000, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	activated_ ( false ),
	timer_ ( 0 )
{};

WindowMonsterSprite::~WindowMonsterSprite() {};

void WindowMonsterSprite::customUpdate( LevelState& level_state )
{
	if ( activated_ )
	{
		if ( animationIsDone() )
		{
			switch ( direction_x_ )
			{
				case ( Direction::Horizontal::LEFT ):
				{
					moveLeft();
					graphics_->rotation_ -= ROTATION_SPEED;
				}
				break;
				case ( Direction::Horizontal::RIGHT ):
				{
					moveRight();
					graphics_->rotation_ += ROTATION_SPEED;
				}
				break;
			}
		}
		else
		{
			if ( timer_ >= 8 )
			{
				graphics_->current_frame_x_ += FRAME_SIZE;
				if ( animationIsDone() )
				{
					changeMovement( SpriteMovement::Type::GROUNDED );
					Audio::playSound( Audio::SoundType::BOP );
				}
				else
				{
					timer_ = 0;
				}
			}
			else
			{
				++timer_;
			}
		}
	}
};

void WindowMonsterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( animationIsDone() && them.hasType( SpriteType::HERO ) )
	{
		if ( them.rightSubPixels() < hit_box_.x )
		{
			direction_x_ = Direction::Horizontal::LEFT;
		}
		else if ( them.hit_box_.x > rightSubPixels() )
		{
			direction_x_ = Direction::Horizontal::RIGHT;
		}

		if ( their_collision.collideAny() )
		{
			level_state.health().hurt();
		}
	}
	else if ( !activated_ && them.hasType( SpriteType::HERO ) )
	{
		if
		(
			( them.hit_box_.y < bottomSubPixels() + Unit::BlocksToSubPixels( 3 ) && them.bottomSubPixels() > them.hit_box_.y - Unit::BlocksToSubPixels( 3 ) ) &&
			(
				( them.rightSubPixels() > hit_box_.x - Unit::BlocksToSubPixels( 2 ) && them.hit_box_.x < rightSubPixels() ) ||
				( them.hit_box_.x < rightSubPixels() + Unit::BlocksToSubPixels( 2 ) && them.rightSubPixels() > hit_box_.x )
			)
		)
		{
			activated_ = true;
		}
	}
};

bool WindowMonsterSprite::animationIsDone() const
{
	return graphics_->current_frame_x_ == FRAME_SIZE * 2;
}
