#pragma once

class Map;
class Sprite;

#include "direction.hpp"
#include "unit.hpp"
#include <SDL2/SDL.h>
#include "simple_camera.hpp"

class Camera
{
	public:
		enum class Type
		{
			NORMAL,
			CENTER_X,
			CENTER_Y,
			CENTER_BOTH,
			SCROLL_LOCK
		};

		constexpr Camera
		(
			int x = 0,
			int y = 0,
			int width = Unit::WINDOW_WIDTH_BLOCKS,
			int height = Unit::WINDOW_HEIGHT_BLOCKS - 2,
			int x_offset = 0,
			int y_offset = 0
		)
		:
			changed_ ( true ),
			prev_x_ ( -87654 ),
			prev_y_ ( -87654 ),
			shake_amount_ ( 0 ),
			shake_dir_ ( Direction::Horizontal::__NULL ),
			position_before_scroll_ ( -1 ),
			scroll_dir_ ( Direction::Simple::__NULL ),
			camera_ ( x, y, width, height, x_offset, y_offset )
		{};

		constexpr sdl2::SDLRect rect() const
		{
			return { camera_.x_, camera_.y_, widthPixels(), heightPixels() };
		};

		constexpr int widthBlocks() const
		{
			return camera_.width_;
		};

		constexpr int heightBlocks() const
		{
			return camera_.height_;
		};

		constexpr int widthPixels() const
		{
			return Unit::BlocksToPixels( camera_.width_ );
		};

		constexpr int heightPixels() const
		{
			return Unit::BlocksToPixels( camera_.height_ );
		};

		constexpr int x() const
		{
			return camera_.x_;
		};

		constexpr int y() const
		{
			return camera_.y_;
		};

		constexpr int xOffset() const
		{
			return camera_.x_offset_;
		};

		constexpr int yOffset() const
		{
			return camera_.y_offset_;
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
			return camera_.x_ + widthPixels();
		};

		constexpr int bottom() const
		{
			return camera_.y_ + heightPixels();
		};

		constexpr int left() const
		{
			return camera_.x_ + xOffset();
		};

		constexpr int top() const
		{
			return camera_.y_ + yOffset();
		};

		constexpr int boundaryTop() const
		{
			return camera_.y_ + ( heightPixels() * .25 );
		};

		constexpr int boundaryTopWeaker() const
		{
			return camera_.y_ + ( heightPixels() * .25 );
		};

		constexpr int boundaryCenterY() const
		{
			return camera_.y_ + ( heightPixels() * .5 );
		};

		constexpr int boundaryBottom() const
		{
			return camera_.y_ + ( heightPixels() * .75 );
		};

		constexpr int boundaryLeft() const
		{
			return camera_.x_ + ( widthPixels() * .35 );
		};

		constexpr int boundaryCenterX() const
		{
			return camera_.x_ + ( widthPixels() * .5 );
		};

		constexpr int boundaryRight() const
		{
			return camera_.x_ + ( widthPixels() * .65 );
		};

		constexpr bool offscreenTop( const sdl2::SDLRect& r, int padding = 0 ) const
		{
			return r.y + r.h < Unit::PixelsToSubPixels( y() - padding );
		};

		constexpr bool offscreenLeft( const sdl2::SDLRect& r, int padding = 0 ) const
		{
			return r.x + r.w < Unit::PixelsToSubPixels( x() - padding );
		};

		constexpr bool offscreenBottom( const sdl2::SDLRect& r, int padding = 0 ) const
		{
			return r.y > Unit::PixelsToSubPixels( bottom() + padding );
		};

		constexpr bool offscreenRight( const sdl2::SDLRect& r, int padding = 0 ) const
		{
			return r.x > Unit::PixelsToSubPixels( right() + padding );
		};

		constexpr bool offscreenPartwayTop( const sdl2::SDLRect& r ) const
		{
			return r.y < y();
		};

		constexpr bool offscreenPartwayRight( const sdl2::SDLRect& r ) const
		{
			return r.x + r.h > widthPixels();
		};

		constexpr bool offscreenPartwayBottom( const sdl2::SDLRect& r ) const
		{
			return r.y + r.w > heightPixels();
		};

		constexpr bool offscreenPartwayLeft( const sdl2::SDLRect& r ) const
		{
			return r.x < x();
		};

		constexpr int relativeX( const sdl2::SDLRect& r ) const
		{
			return r.x - x() + xOffset();
		};

		constexpr int relativeY( const sdl2::SDLRect& r ) const
		{
			return r.y - y() + yOffset();
		};

		constexpr int relativeX( int n ) const
		{
			return n - x() + xOffset();
		};

		constexpr int relativeY( int n ) const
		{
			return n - y() + yOffset();
		};

		constexpr sdl2::SDLRect relativeRect( const sdl2::SDLRect& r ) const
		{
			return { relativeX( r ), relativeY( r ), r.w, r.h };
		};

		bool onscreen( const sdl2::SDLRect& r, int padding = 0 )  const;
		bool onscreenPixels( const sdl2::SDLRect& r, int padding = 0 )  const;
		bool offscreen( const sdl2::SDLRect& r, int padding = 0, Direction::Simple direction = Direction::Simple::__NULL ) const;

		bool changed() const;
		void update();
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
		bool testPause() const;


	private:
		bool changed_;
		int prev_x_;
		int prev_y_;
		int shake_amount_;
		int position_before_scroll_;
		Direction::Horizontal shake_dir_;
		Direction::Simple scroll_dir_;
		SimpleCamera camera_;

		void move( const Sprite& o, const Type type );
		void contain( const Map& m );
		void moveXNormal( const Sprite& o );
		void moveYNormal( const Sprite& o );
		void moveXCenter( const Sprite& o );
		void moveYCenter( const Sprite& o );
		void updateShaking();
};
