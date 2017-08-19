#pragma once

#include "unit.hpp"



class OWCamera
{
	public:
		static constexpr int W = Unit::WINDOW_WIDTH_PIXELS;
		static constexpr int H = Unit::WINDOW_HEIGHT_PIXELS - 32;
		static constexpr int NORMAL_SPEED = 5;
		static constexpr int FAST_SPEED = 10;

		constexpr OWCamera()
		:
			x_ ( 0 ),
			y_ ( 0 ),
			speed_ ( 5 )
		{};

		void adjust( int x, int y, int w, int h, int map_w, int map_h );
		void center( int x, int y, int w, int h, int map_w, int map_h );
		bool backToHero( int x, int y, int w, int h, int map_w, int map_h );
		void move( int map_w, int map_h );

		constexpr sdl::rect relative( const sdl::rect& r ) const
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
		int speed_;

		constexpr int leftBoundary()   const { return ( int )( W * 0.25 ) + x_; };
		constexpr int rightBoundary()  const { return ( int )( W * 0.75 ) + x_; };
		constexpr int topBoundary()    const { return ( int )( H * 0.25 ) + y_; };
		constexpr int bottomBoundary() const { return ( int )( H * 0.75 ) + y_; };
	
		void bounds( int map_w, int map_h );
};