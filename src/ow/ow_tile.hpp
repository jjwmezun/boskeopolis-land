#pragma once

#include "sprite_graphics.h"

class AnimatedGraphics;

class OWCamera;
class OWHero;

class OWTile
{
	public:
		OWTile( int type, int x, int y, const AnimatedGraphics& gfx );
		void render( const OWCamera& camera ) const;
		void highlight( const OWCamera& camera ) const;

		void interact( OWHero& hero, const Collision& collision ) const;
		sdl2::SDLRect coords() const;

		int type_;

	private:
		static constexpr int SIZE = 16;
		static constexpr int TEXTURE_WIDTH = 16;

		const int x_;
		const int y_;
	
		sdl2::SDLRect src() const;
		sdl2::SDLRect dest( const OWCamera& camera ) const;
	
		const SpriteGraphics* gfx_;
};