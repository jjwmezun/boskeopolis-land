#pragma once

#include "animated_graphics.hpp"

class Collision;


class OWCamera;
class OWLevel;

class OWHero
{
	public:
		static constexpr int W = 12;
		static constexpr int H = 14;

		OWHero( int x = 0, int y = 0 );
		void update();
		void render( const OWCamera& camera );
		void placeOnLv( const OWLevel& lv );

		int x() const;
		int y() const;
		int right() const;
		int bottom() const;
		void collideStop( const Collision& collision );
		sdl2::SDLRect coords() const;

	private:
		AnimatedGraphics gfx_;
		int x_;
		int y_;
		int speed_;
};