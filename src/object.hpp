#pragma once

class Camera;
class Collision;

#include "direction.hpp"
#include <SDL2/SDL.h>
#include "unit.hpp"

class Object
{
	public:
		Object
		(
			int x      = 0,
			int y      = 0,
			int width  = 16,
			int height = 16
		);

		void changeX( int new_x_pixels );
		void changeY( int new_y_pixels );
		void addToX ( int addition_x_pixels );
		void addToY ( int addition_y_pixels );

		const sdl::rect& hitBox() const;

		int xPixels()    const;
		int yPixels()    const;
		int xSubPixels() const;
		int ySubPixels() const;

		int heightPixels()    const;
		int widthPixels()     const;
		int heightSubPixels() const;
		int widthSubPixels()  const;

		int halfWidthSubPixels()  const;
		int halfHeightSubPixels() const;
		int centerXSubPixels()    const;
		int centerYSubPixels()    const;

		int halfWidthPixels()  const;
		int halfHeightPixels() const;
		int centerXPixels() const;
		int centerYPixels() const;

		int leftSubPixels()   const;
		int rightSubPixels()  const;
		int topSubPixels()    const;
		int bottomSubPixels() const;

		int rightPixels()  const;
		int bottomPixels() const;


	protected:
		sdl::rect hit_box_;
};