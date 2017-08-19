#pragma once

class Sprite;

#include "counter.hpp"
#include "sprite_graphics.hpp"
#include "timer_repeat.hpp"

class FaucetGraphics : public SpriteGraphics
{
	public:
		FaucetGraphics();
		~FaucetGraphics();

		void update( Sprite& sprite );
		void render( const sdl::rect& bound_box, const Camera* camera, bool priority ) const override;
		
	private:
		TimerRepeat animation_timer_;
};