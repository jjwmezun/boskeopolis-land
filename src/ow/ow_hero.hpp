#pragma once

#include "image_graphics.hpp"
#include "dpoint.hpp"
#include "timer_repeat_t2.hpp"
#include <vector>

class Collision;
class OWCamera;
class OWLevel;
class OWTileMap;

class OWHero final
{
	public:
		OWHero( int x = 0, int y = 0 );
		void update( const OWTileMap& tilemap, const sdl2::SDLRect& bounds );
		void render( const OWCamera& camera );
		void updateGraphics( const sdl2::SDLRect& bounds );
		void setPosition( int x, int y, const sdl2::SDLRect& bounds );
		void updateAnimation();

		int x() const;
		int y() const;
		const DPoint& getPosition() const;
		const sdl2::SDLRect& getGraphicsBox() const;

	private:
		void keepInBounds( const sdl2::SDLRect& bounds );
		void updateGraphicsX( const sdl2::SDLRect& bounds );
		void updateGraphicsY( const sdl2::SDLRect& bounds );

		DPoint position_;
		ImageGraphics graphics_;
		sdl2::SDLRect absolute_graphics_box_;
		double x_speed_;
		double y_speed_;
		TimerRepeatT2<8> animation_timer_;
};