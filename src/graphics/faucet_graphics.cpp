#include "faucet_sprite.hpp"
#include "faucet_graphics.hpp"
#include "sprite_graphics.hpp"

FaucetGraphics::FaucetGraphics()
:
	animation_timer_ ()
{};

void FaucetGraphics::update( const FaucetSprite& sprite, SpriteGraphics* graphics )
{
	if ( animation_timer_.hit() )
	{
		graphics->current_frame_x_ += 16;
		if ( graphics->current_frame_x_ > 240 )
		{
			graphics->current_frame_x_ = 0;
		}
	}
	else if ( animation_timer_.on() )
	{
		animation_timer_.update();
	}
	else
	{
		animation_timer_.start();
	}
	graphics->current_frame_y_ = sprite.getHits() * 80;
};

void FaucetGraphics::render( const SpriteGraphics* graphics, const Camera* camera, bool priority, int x, int y ) const
{
	if ( priority )
	{
		graphics->masterRender( { x -= ( 16 * 3 ), y + 16, 16, 80 }, graphics->current_frame_x_, graphics->current_frame_y_, camera, !priority );
	}
	else
	{
		graphics->masterRender( { x, y, 16, 16 }, 256, 0, camera, priority );
	}
}
