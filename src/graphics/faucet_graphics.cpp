#include "faucet_graphics.hpp"
#include "sprite.hpp"

FaucetGraphics::FaucetGraphics()
:
	SpriteGraphics ( "sprites/faucet.png", 0, 0 ),
	animation_timer_ ()
{};

FaucetGraphics::~FaucetGraphics() {};

void FaucetGraphics::update( Sprite& sprite )
{
	if ( animation_timer_.hit() )
	{
		current_frame_x_ += 16;
		
		if ( current_frame_x_ > 240 )
		{
			current_frame_x_ = 0;
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
};

void FaucetGraphics::render( const sdl2::SDLRect& bound_box, const Camera* camera, bool priority, Uint8 alpha ) const
{
	masterRender( bound_box, 256, 0, camera, priority, alpha );

	if ( priority )
	{
		auto waterfall_box = bound_box;
		waterfall_box.x -= ( 16 * 3 );

		for ( int i = 0; i < 4; ++i )
		{
			waterfall_box.y += 16;
			masterRender( waterfall_box, current_frame_x_, current_frame_y_ * 32, camera, false, alpha );
		}
		waterfall_box.y += 16;
		masterRender( waterfall_box, current_frame_x_, ( current_frame_y_ * 32 ) + 16, camera, false, alpha );
	}
}