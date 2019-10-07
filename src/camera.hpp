#pragma once

class Map;
class Sprite;

#include "direction.hpp"
#include "line.hpp"
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

		constexpr inline sdl2::SDLRect rect() const
		{
			return { camera_.x_, camera_.y_, widthPixels(), heightPixels() };
		};

		constexpr inline int widthBlocks() const
		{
			return camera_.width_;
		};

		constexpr inline int heightBlocks() const
		{
			return camera_.height_;
		};

		constexpr inline int widthPixels() const
		{
			return Unit::BlocksToPixels( camera_.width_ );
		};

		constexpr inline int heightPixels() const
		{
			return Unit::BlocksToPixels( camera_.height_ );
		};

		constexpr inline int x() const
		{
			return camera_.x_;
		};

		constexpr inline int y() const
		{
			return camera_.y_;
		};

		constexpr inline int xOffset() const
		{
			return camera_.x_offset_;
		};

		constexpr inline int yOffset() const
		{
			return camera_.y_offset_;
		};

		constexpr inline int prevX() const
		{
			return prev_x_;
		};

		constexpr inline int prevY() const
		{
			return prev_y_;
		};

		constexpr inline int right() const
		{
			return camera_.x_ + widthPixels();
		};

		constexpr inline int bottom() const
		{
			return camera_.y_ + heightPixels();
		};

		constexpr inline int left() const
		{
			return camera_.x_ + xOffset();
		};

		constexpr int top() const
		{
			return camera_.y_ + yOffset();
		};

		constexpr inline int boundaryTop() const
		{
			return camera_.y_ + ( heightPixels() * .25 );
		};

		constexpr inline int boundaryTopWeaker() const
		{
			return camera_.y_ + ( heightPixels() * .25 );
		};

		constexpr inline int boundaryCenterY() const
		{
			return camera_.y_ + ( heightPixels() * .5 );
		};

		constexpr inline int boundaryBottom() const
		{
			return camera_.y_ + ( heightPixels() * .75 );
		};

		constexpr inline int boundaryLeft() const
		{
			return camera_.x_ + ( widthPixels() * .35 );
		};

		constexpr inline int boundaryCenterX() const
		{
			return camera_.x_ + ( widthPixels() * .5 );
		};

		constexpr inline int boundaryRight() const
		{
			return camera_.x_ + ( widthPixels() * .65 );
		};

		constexpr inline bool offscreenTop( sdl2::SDLRect r, int padding = 0 ) const
		{
			return r.y + r.h < Unit::PixelsToSubPixels( y() - padding );
		};

		constexpr inline bool offscreenLeft( sdl2::SDLRect r, int padding = 0 ) const
		{
			return r.x + r.w < Unit::PixelsToSubPixels( x() - padding );
		};

		constexpr inline bool offscreenBottom( sdl2::SDLRect r, int padding = 0 ) const
		{
			return r.y > Unit::PixelsToSubPixels( bottom() + padding );
		};

		constexpr inline bool offscreenRight( sdl2::SDLRect r, int padding = 0 ) const
		{
			return r.x > Unit::PixelsToSubPixels( right() + padding );
		};

		constexpr inline bool offscreenPartwayTop( sdl2::SDLRect r ) const
		{
			return r.y < y();
		};

		constexpr inline bool offscreenPartwayRight( sdl2::SDLRect r ) const
		{
			return r.x + r.h > widthPixels();
		};

		constexpr inline bool offscreenPartwayBottom( sdl2::SDLRect r ) const
		{
			return r.y + r.w > heightPixels();
		};

		constexpr inline bool offscreenPartwayLeft( sdl2::SDLRect r ) const
		{
			return r.x < x();
		};

		constexpr inline int relativeX( sdl2::SDLRect r ) const
		{
			return r.x - x() + xOffset();
		};

		constexpr inline int relativeY( sdl2::SDLRect r ) const
		{
			return r.y - y() + yOffset();
		};

		constexpr inline int relativeX( int n ) const
		{
			return n - x() + xOffset();
		};

		constexpr inline int relativeY( int n ) const
		{
			return n - y() + yOffset();
		};

		constexpr inline sdl2::SDLRect relativeRect( sdl2::SDLRect r ) const
		{
			return { relativeX( r ), relativeY( r ), r.w, r.h };
		};

		constexpr inline Point relativePoint( Point p ) const
		{
			return { relativeX( p.x ), relativeY( p.y ) };
		};

		constexpr inline Line relativeLine( Line l ) const
		{
			return { relativePoint( l.p1 ), relativePoint( l.p2 ) };
		};

		bool onscreen( sdl2::SDLRect r, int padding = 0 )  const;
		bool onscreenPixels( sdl2::SDLRect r, int padding = 0 )  const;
		bool offscreen( sdl2::SDLRect r, int padding = 0, Direction::Simple direction = Direction::Simple::__NULL ) const;

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
