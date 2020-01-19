#pragma once

#include "image_graphics.hpp"
#include "dpoint.hpp"

class Collision;
class OWCamera;
class OWLevel;

class OWHero
{
	public:
		OWHero( int x = 0, int y = 0 );
		void update( const sdl2::SDLRect& bounds );
		void render( const OWCamera& camera );
		void updateGraphics( const sdl2::SDLRect& bounds );

		int x() const;
		int y() const;
		const Point getPosition() const;
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
		char scratch_[ 1800 ];
};