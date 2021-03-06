#pragma once

#include "mezun_sdl2.hpp"
#include "ow_inventory.hpp"
#include "unit.hpp"

class DPoint;

class OWCamera final
{
	public:
		static constexpr int WIDTH = Unit::WINDOW_WIDTH_PIXELS - 12;
		static constexpr int HEIGHT = Unit::WINDOW_HEIGHT_PIXELS - OWInventory::HEIGHT - 12;
		static constexpr int LEFT_BOUNDARY_POINT = ( int )( ( double )( WIDTH ) * 0.25 );
		static constexpr int RIGHT_BOUNDARY_POINT = ( int )( ( double )( WIDTH ) * 0.75 );
		static constexpr int TOP_BOUNDARY_POINT = ( int )( ( double )( HEIGHT ) * 0.25 );
		static constexpr int BOTTOM_BOUNDARY_POINT = ( int )( ( double )( HEIGHT ) * 0.75 );
		static constexpr int NORMAL_SPEED = 5;
		static constexpr int FAST_SPEED = NORMAL_SPEED * 2;

		constexpr OWCamera( int bounds_w = WIDTH, int bounds_h = HEIGHT )
		:
			coords_ ( 0, 0, WIDTH, HEIGHT ),
			bounds_w_ ( bounds_w ),
			bounds_h_ ( bounds_h )
		{};

		constexpr void setBounds( int bounds_w, int bounds_h )
		{
			bounds_w_ = bounds_w;
			bounds_h_ = bounds_h;
		};

		void adjust( const sdl2::SDLRect& target );
		void center( const DPoint& target );
		bool moveAutomaticallyToTarget( const DPoint& target, int speed = FAST_SPEED );
		void move();

		constexpr sdl2::SDLRect relative( const sdl2::SDLRect& r ) const
		{
			return
			{
				r.x - coords_.x,
				r.y - coords_.y,
				r.w,
				r.h
			};
		};
	
		constexpr int x() const { return coords_.x; };
		constexpr int y() const { return coords_.y; };
		constexpr const sdl2::SDLRect& getBox() const { return coords_; };

	private:
		sdl2::SDLRect coords_;
		int bounds_w_;
		int bounds_h_;

		constexpr int leftBoundary()   const { return LEFT_BOUNDARY_POINT   + coords_.x; };
		constexpr int rightBoundary()  const { return RIGHT_BOUNDARY_POINT  + coords_.x; };
		constexpr int topBoundary()    const { return TOP_BOUNDARY_POINT    + coords_.y; };
		constexpr int bottomBoundary() const { return BOTTOM_BOUNDARY_POINT + coords_.y; };
	
		void keepInBounds();
};