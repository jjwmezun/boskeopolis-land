#include "collision.hpp"
#include "object.hpp"
#include "unit.hpp"
#include <iostream>

Object::Object
(
	int x,
	int y,
	int width,
	int height
)
:
	hit_box_ ( { Unit::PixelsToSubPixels( x ), Unit::PixelsToSubPixels( y ), Unit::PixelsToSubPixels( width ), Unit::PixelsToSubPixels( height ) } )
{};

const sdl::rect& Object::hitBox() const
{
	return hit_box_;
};

void Object::changeX( int new_x_pixels )
{
	hit_box_.x = new_x_pixels;
};

void Object::changeY( int new_y_pixels )
{
	hit_box_.y = new_y_pixels;
};

void Object::addToX( int addition_x_pixels )
{
	hit_box_.x += addition_x_pixels;
};

void Object::addToY( int addition_y_pixels )
{
	hit_box_.y += addition_y_pixels;
};

int Object::heightPixels() const
{
	return Unit::SubPixelsToPixels( hit_box_.h );
};

int Object::widthPixels() const
{
	return Unit::SubPixelsToPixels( hit_box_.w );
};

int Object::heightSubPixels() const
{
	return hit_box_.h;
};

int Object::widthSubPixels() const
{
	return hit_box_.w;
};

int Object::xPixels() const
{
	return Unit::SubPixelsToPixels( hit_box_.x );
};

int Object::yPixels() const
{
	return Unit::SubPixelsToPixels( hit_box_.y );
};

int Object::xSubPixels() const
{
	return hit_box_.x;
};

int Object::ySubPixels() const
{
	return hit_box_.y;
};

int Object::halfWidthSubPixels() const
{
	return round( hit_box_.w / 2 );
};

int Object::halfHeightSubPixels() const
{
	return round( hit_box_.h / 2 );
};

int Object::centerXSubPixels() const
{
	return xSubPixels() + halfWidthSubPixels();
};

int Object::centerYSubPixels() const
{
	return ySubPixels() + halfHeightSubPixels();
};

int Object::halfWidthPixels() const
{
	return round( widthPixels() / 2 );
};

int Object::halfHeightPixels() const
{
	return round( heightPixels() / 2 );
};

int Object::centerXPixels() const
{
	return xPixels() + halfWidthPixels();
};

int Object::centerYPixels() const
{
	return yPixels() + halfHeightPixels();
};

int Object::leftSubPixels() const
{
	return hit_box_.x;
};

int Object::rightSubPixels() const
{
	return hit_box_.x + hit_box_.w;
};

int Object::topSubPixels() const
{
	return hit_box_.y;
};

int Object::bottomSubPixels() const
{
	return hit_box_.y + hit_box_.h;
};

int Object::rightPixels() const
{
	return Unit::SubPixelsToPixels( rightSubPixels() );
};

int Object::bottomPixels() const
{
	return Unit::SubPixelsToPixels( bottomSubPixels() );
};