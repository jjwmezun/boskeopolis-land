#pragma once

#include "sprite_graphics.hpp"

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
		sdl::rect coords() const;

		int type_;
		
		void changeType( int type, const SpriteGraphics& gfx );

	private:
		static constexpr int SIZE = 16;
		static constexpr int TEXTURE_WIDTH = 16;

		const int x_;
		const int y_;
		const SpriteGraphics* gfx_;
	
		sdl::rect src() const;
		sdl::rect dest( const OWCamera& camera ) const;
		void setGFX( int type, const SpriteGraphics& gfx );
};