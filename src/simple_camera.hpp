#pragma once

#include "direction.hpp"
#include "mezun_sdl2.hpp"
#include "unit.hpp"
#include <SDL2/SDL.h>

struct SimpleCamera
{
	constexpr SimpleCamera
	(
		int x = 0,
		int y = 0,
		int width = Unit::WINDOW_WIDTH_BLOCKS,
		int height = Unit::WINDOW_HEIGHT_BLOCKS - 2,
		int x_offset = 0,
		int y_offset = 0
	)
	:
		x_( x ),
		y_( y ),
		width_( width ),
		height_( height ),
		x_offset_ ( x_offset ),
		y_offset_ ( y_offset )
	{};

	constexpr sdl2::SDLRect rect() const
	{
		return { x_, y_, widthPixels(), heightPixels() };
	};

	constexpr int widthBlocks() const
	{
		return width_;
	};

	constexpr int heightBlocks() const
	{
		return height_;
	};

	constexpr int widthPixels() const
	{
		return Unit::BlocksToPixels( width_ );
	};

	constexpr int heightPixels() const
	{
		return Unit::BlocksToPixels( height_ );
	};

	constexpr int right() const
	{
		return x_ + widthPixels();
	};

	constexpr int bottom() const
	{
		return y_ + heightPixels();
	};

	constexpr int left() const
	{
		return x_ + x_offset_;
	};

	constexpr int top() const
	{
		return y_ + y_offset_;
	};

	constexpr int boundaryTop() const
	{
		return y_ + ( heightPixels() * .25 );
	};

	constexpr int boundaryTopWeaker() const
	{
		return y_ + ( heightPixels() * .25 );
	};

	constexpr int boundaryCenterY() const
	{
		return y_ + ( heightPixels() * .5 );
	};

	constexpr int boundaryBottom() const
	{
		return y_ + ( heightPixels() * .75 );
	};

	constexpr int boundaryLeft() const
	{
		return x_ + ( widthPixels() * .35 );
	};

	constexpr int boundaryCenterX() const
	{
		return x_ + ( widthPixels() * .5 );
	};

	constexpr int boundaryRight() const
	{
		return x_ + ( widthPixels() * .65 );
	};

	constexpr bool offscreenTop( const sdl2::SDLRect& r, int padding = 0 ) const
	{
		return r.y + r.h < Unit::PixelsToSubPixels( y_ - padding );
	};

	constexpr bool offscreenLeft( const sdl2::SDLRect& r, int padding = 0 ) const
	{
		return r.x + r.w < Unit::PixelsToSubPixels( x_ - padding );
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
		return r.y < y_;
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
		return r.x < x_;
	};

	constexpr int relativeX( const sdl2::SDLRect& r ) const
	{
		return r.x - x_ + x_offset_;
	};

	constexpr int relativeY( const sdl2::SDLRect& r ) const
	{
		return r.y - y_ + y_offset_;
	};

	constexpr int relativeX( int n ) const
	{
		return n - x_ + x_offset_;
	};

	constexpr int relativeY( int n ) const
	{
		return n - y_ + y_offset_;
	};

	constexpr sdl2::SDLRect relativeRect( const sdl2::SDLRect& r ) const
	{
		return { relativeX( r ), relativeY( r ), r.w, r.h };
	};

	bool onscreen( const sdl2::SDLRect& r, int padding = 0 )  const;
	bool onscreenPixels( const sdl2::SDLRect& r, int padding = 0 )  const;
	bool offscreen( const sdl2::SDLRect& r, int padding = 0, Direction::Simple direction = Direction::Simple::__NULL ) const;

	int x_;
	int y_;
	const int x_offset_;
	const int y_offset_;
	const int width_;
	const int height_;
};
