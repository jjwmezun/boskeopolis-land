#pragma once

class Sprite;

#include "counter.hpp"
#include "sprite_graphics.hpp"

class FaucetGraphics : public SpriteGraphics
{
	public:
		FaucetGraphics();
		~FaucetGraphics();

		void update( Sprite& sprite );
		void render( const sdl2::SDLRect& bound_box, const Camera* camera, bool priority, Uint8 alpha ) const override;
};