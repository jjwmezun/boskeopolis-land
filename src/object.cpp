#include "collision.h"
#include "object.h"
#include "unit.h"
#include <iostream>

Object::Object
(
	int x,
	int y,
	int width,
	int height
)
:
	original_hit_box_ ( { Unit::PixelsToSubPixels( x ), Unit::PixelsToSubPixels( y ), Unit::PixelsToSubPixels( width ), Unit::PixelsToSubPixels( height ) } ),
	hit_box_ ( { Unit::PixelsToSubPixels( x ), Unit::PixelsToSubPixels( y ), Unit::PixelsToSubPixels( width ), Unit::PixelsToSubPixels( height ) } )
{};

Object::~Object() {};

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

int Object::prevLeftSubPixels() const
{
	return xPrevSubPixels();
};

int Object::prevRightSubPixels() const
{
	return xPrevSubPixels() + hit_box_.w;
};

int Object::prevTopSubPixels() const
{
	return yPrevSubPixels();
};

int Object::prevBottomSubPixels() const
{
	return yPrevSubPixels() + hit_box_.h;
};

int Object::xPrevSubPixels() const
{
	return x_prev_;
};

int Object::yPrevSubPixels() const
{
	return y_prev_;
};

int Object::xPrevPixels() const
{
	return Unit::SubPixelsToPixels( x_prev_ );
};

int Object::yPrevPixels() const
{
	return Unit::SubPixelsToPixels( y_prev_ );
};

int Object::rightPixels() const
{
	return Unit::SubPixelsToPixels( rightSubPixels() );
};

int Object::bottomPixels() const
{
	return Unit::SubPixelsToPixels( bottomSubPixels() );
};

int Object::prevRightPixels() const
{
	return Unit::SubPixelsToPixels( prevRightSubPixels() );
};

int Object::prevBottomPixels() const
{
	return Unit::SubPixelsToPixels( prevBottomSubPixels() );
};

const sdl2::SDLRect& Object::hitBox() const
{
	return hit_box_;
};

int Object::originalXSubPixels() const
{
	return original_hit_box_.x;
};

int Object::originalYSubPixels() const
{
	return original_hit_box_.y;
};

const sdl2::SDLRect& Object::originalHitBox() const
{
	return original_hit_box_;
};