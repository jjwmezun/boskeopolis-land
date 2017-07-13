#pragma once

#include "unit.h"

class OWCamera
{
	public:
		static constexpr int W = Unit::WINDOW_WIDTH_PIXELS;
		static constexpr int H = Unit::WINDOW_HEIGHT_PIXELS - 32;

		constexpr OWCamera()
		:
			x_ ( 0 ),
			y_ ( 0 )
		{};

		void adjust( int x, int y, int w, int h, int map_w, int map_h );
		void center( int x, int y, int w, int h, int map_w, int map_h );

		constexpr sdl2::SDLRect relative( const sdl2::SDLRect& r ) const
		{
			return
			{
				r.x - x_,
				r.y - y_,
				r.w,
				r.h
			};
		};
	
		constexpr int x() const { return x_; };
		constexpr int y() const { return y_; };

	private:
		int x_;
		int y_;

		constexpr int leftBoundary()   const { return ( int )( W * 0.25 ) + x_; };
		constexpr int rightBoundary()  const { return ( int )( W * 0.75 ) + x_; };
		constexpr int topBoundary()    const { return ( int )( H * 0.25 ) + y_; };
		constexpr int bottomBoundary() const { return ( int )( H * 0.75 ) + y_; };
	
		void bounds( int map_w, int map_h );
};