#include "audio.hpp"
#include "event_system.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"
#include "switch_block_sprite.hpp"

SwitchBlockSprite::SwitchBlockSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/universal.png", 0, 112 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

SwitchBlockSprite::~SwitchBlockSprite() {};

void SwitchBlockSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( !ready_ )
	{
		if ( delay_.done() )
		{
			ready_ = true;
			delay_.stop();
		}
		else if ( delay_.on() )
		{
			delay_.update();
		}
		else
		{
			delay_.start();
		}
	}

	graphics_->current_frame_x_ = ( events.switchOn() ) ? 0 : 16;

	if ( hit_box_.y < original_hit_box_.y )
	{
		hit_box_.y += 500;
	}

	if ( hit_box_.y > original_hit_box_.y )
	{
		hit_box_.y = original_hit_box_.y;
	}
};

void SwitchBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );

		if ( their_collision.collideTop() )
		{
			if ( ready_ )
			{
				events.flipSwitch();
				ready_ = false;
				Audio::playSound( Audio::SoundType::SWITCH );
			}

			hit_box_.y -= their_collision.overlapYTop() / 2;

		}
	}
};
