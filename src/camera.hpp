#pragma once

class Map;
class Sprite;

#include "direction.hpp"
#include "line.hpp"
#include "unit.hpp"
#include <SDL2/SDL.h>

class Camera final
{
	public:
		enum class Type
		{
			NORMAL,
			CENTER_X,
			CENTER_Y,
			CENTER_BOTH,
			SCROLL_LOCK,
			PRIORITIZE_TOP
		};

		constexpr Camera
		(
			int width = Unit::WINDOW_WIDTH_PIXELS,
			int height = Unit::WINDOW_HEIGHT_PIXELS - 32,
			int x_offset = 0,
			int y_offset = 0,
			int container_width = Unit::WINDOW_WIDTH_PIXELS,
			int container_height = Unit::WINDOW_HEIGHT_PIXELS
		)
		:
			changed_ ( true ),
			position_before_scroll_ ( -1 ),
			scroll_dir_ ( Direction::Simple::__NULL ),
			x_ ( -1 ),
			y_ ( -1 ),
			width_ ( width ),
			height_ ( height ),
			x_offset_ ( x_offset ),
			y_offset_ ( y_offset ),
			container_width_ ( container_width ),
			container_height_ ( container_height ),
			prev_x_ ( -87654 ),
			prev_y_ ( -87654 ),
			shake_amount_ ( 0 ),
			shake_dir_ ( Direction::Horizontal::__NULL )
		{};

		constexpr sdl2::SDLRect rect() const
		{
			return { x_, y_, container_width_, container_height_ };
		};

		constexpr int width() const
		{
			return width_;
		};

		constexpr int height() const
		{
			return height_;
		};

		constexpr int x() const
		{
			return x_;
		};

		constexpr int y() const
		{
			return y_;
		};

		constexpr int xOffset() const
		{
			return x_offset_;
		};

		constexpr int yOffset() const
		{
			return y_offset_;
		};

		constexpr int prevX() const
		{
			return prev_x_;
		};

		constexpr int prevY() const
		{
			return prev_y_;
		};

		constexpr int right() const
		{
			return x_ + width_;
		};

		constexpr int bottom() const
		{
			return y_ + height_;
		};

		constexpr int screenRight() const
		{
			return x_ + container_width_;
		};

		constexpr int screenBottom() const
		{
			return y_ + container_height_;
		};

		constexpr int left() const
		{
			return x_ + xOffset();
		};

		constexpr int top() const
		{
			return y_ + yOffset();
		};

		constexpr int boundaryTop() const
		{
			return y_ + ( height_ * .25 );
		};

		constexpr int boundaryTopWeaker() const
		{
			return y_ + ( height_ * .25 );
		};

		constexpr int boundaryCenterY() const
		{
			return y_ + ( height_ * .5 );
		};

		constexpr int boundaryBottom() const
		{
			return y_ + ( height_ * .75 );
		};

		constexpr int boundaryLeft() const
		{
			return x_ + ( width_ * .35 );
		};

		constexpr int boundaryCenterX() const
		{
			return x_ + ( width_ * .5 );
		};

		constexpr int boundaryRight() const
		{
			return x_ + ( width_ * .65 );
		};

		constexpr bool offscreenTop( sdl2::SDLRect r, int padding = 0 ) const
		{
			return r.y + r.h < Unit::PixelsToSubPixels( y_ - padding );
		};

		constexpr bool offscreenLeft( sdl2::SDLRect r, int padding = 0 ) const
		{
			return r.x + r.w < Unit::PixelsToSubPixels( x_ - padding );
		};

		constexpr bool offscreenBottom( sdl2::SDLRect r, int padding = 0 ) const
		{
			return r.y > Unit::PixelsToSubPixels( screenBottom() + padding );
		};

		constexpr bool offscreenRight( sdl2::SDLRect r, int padding = 0 ) const
		{
			return r.x > Unit::PixelsToSubPixels( screenRight() + padding );
		};

		constexpr bool offscreenPartwayTop( sdl2::SDLRect r ) const
		{
			return r.y < y_;
		};

		constexpr bool offscreenPartwayRight( sdl2::SDLRect r ) const
		{
			return r.x + r.h > container_width_;
		};

		constexpr bool offscreenPartwayBottom( sdl2::SDLRect r ) const
		{
			return r.y + r.w > container_height_;
		};

		constexpr bool offscreenPartwayLeft( sdl2::SDLRect r ) const
		{
			return r.x < x_;
		};

		constexpr int relativeX( sdl2::SDLRect r ) const
		{
			return r.x - x_ + xOffset();
		};

		constexpr int relativeY( sdl2::SDLRect r ) const
		{
			return r.y - y_ + yOffset();
		};

		constexpr int relativeX( int n ) const
		{
			return n - x_ + xOffset();
		};

		constexpr int relativeY( int n ) const
		{
			return n - y_ + yOffset();
		};

		constexpr sdl2::SDLRect relativeRect( sdl2::SDLRect r ) const
		{
			return { relativeX( r ), relativeY( r ), r.w, r.h };
		};

		constexpr Point relativePoint( Point p ) const
		{
			return { relativeX( p.x ), relativeY( p.y ) };
		};

		constexpr Line relativeLine( Line l ) const
		{
			return { relativePoint( l.p1 ), relativePoint( l.p2 ) };
		};
		
		constexpr bool changed() const
		{
			return changed_;
		};

		constexpr void update()
		{
			changed_ = false;
		};

		constexpr bool testPause() const
		{
			return scroll_dir_ != Direction::Simple::__NULL;
		};

		bool onscreen( sdl2::SDLRect r, int padding = 0 )  const;
		bool onscreenPixels( sdl2::SDLRect r, int padding = 0 )  const;
		bool offscreen( sdl2::SDLRect r, int padding = 0, Direction::Simple direction = Direction::Simple::__NULL ) const;

		void scroll( const Map& m );
		void adjust( const Sprite& o, const Map& m );
		void adjustCart( const Sprite& o, const Map& m );
		void setPrev();
		void moveDown( int amount = 1 );
		void moveUp( int amount = 1 );
		void moveLeft( int amount = 1 );
		void moveRight( int amount = 1 );
		void setPosition( int x, int y );
		void startShaking();

	private:
		bool changed_;
		Direction::Horizontal shake_dir_;
		Direction::Simple scroll_dir_;
		int x_;
		int y_;
		int x_offset_;
		int y_offset_;
		int width_;
		int height_;
		int container_width_;
		int container_height_;
		int prev_x_;
		int prev_y_;
		int shake_amount_;
		int position_before_scroll_;

		void move( const Sprite& o, const Type type );
		void contain( const Map& m );
		void moveXNormal( const Sprite& o );
		void moveYNormal( const Sprite& o );
		void moveXCenter( const Sprite& o );
		void moveYCenter( const Sprite& o );
		void updateShaking();
};
