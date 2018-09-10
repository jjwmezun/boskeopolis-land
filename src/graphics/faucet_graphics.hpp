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
		void render( const sdl2::SDLRect& bound_box, const Camera* camera = nullptr, bool priority = false, SDL_Texture* texture = nullptr ) const override;

	private:
		TimerRepeat animation_timer_;
};
