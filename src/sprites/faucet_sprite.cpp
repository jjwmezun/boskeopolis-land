#include "collision.hpp"
#include "event_system.hpp"
#include "faucet_sprite.hpp"
#include "faucet_graphics.hpp"
#include <iostream>

FaucetSprite::FaucetSprite( int x, int y )
:
	Sprite( std::make_unique<FaucetGraphics> (), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false, true, false, 0.0, false, true ),
	hits_ ( 0 ),
	invincibility_ ( 0 ),
	slide_lock_ ( false )
{};

FaucetSprite::~FaucetSprite() {};

void FaucetSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( hits_ >= 3 && invincibility_ <= 0 )
	{
		events.win();
	}
	else if ( invincibility_ > 0 )
	{
		--invincibility_;
	}
	
	graphics_->current_frame_y_ = hits_;

	if ( invincibility_ % 4 == 1 )
	{
		graphics_->visible_ = false;
	}
	else
	{
		graphics_->visible_ = true;
	}
};

void FaucetSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health )
{
	if ( their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
		
		if ( them.hasType( SpriteType::HERO ) )
		{
			if ( them.isSlidingPrev() )
			{
				if ( !slide_lock_ && invincibility_ <= 0 )
				{
					++hits_;
					invincibility_ = 30;
				}
				slide_lock_ = true;
			}
		}
	}
	else if ( them.hasType( SpriteType::HERO ) )
	{
		slide_lock_ = false;
	}
};	