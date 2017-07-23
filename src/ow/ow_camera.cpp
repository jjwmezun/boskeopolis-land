#include "input.hpp"
#include <iostream>
#include "ow_camera.hpp"

void OWCamera::adjust( int x, int y, int w, int h, int map_w, int map_h )
{
	if ( x < leftBoundary() )
	{
		x_ -= leftBoundary() - x;
	}
	else if ( x + w > rightBoundary() )
	{
		x_ += x + w - rightBoundary();
	}

	if ( y < topBoundary() )
	{
		y_ -= topBoundary() - y;
	}
	else if ( y + h > bottomBoundary() )
	{
		y_ += y + h - bottomBoundary();
	}
	
	bounds( map_w, map_h );
};


void OWCamera::center( int x, int y, int w, int h, int map_w, int map_h )
{
	x_ = x - ( W / 2 ) - ( w / 2 );
	y_ = y - ( H / 2 ) - ( h / 2 );
	bounds( map_w, map_h );
};

bool OWCamera::backToHero( int x, int y, int w, int h, int map_w, int map_h )
{
	bool close_nough_x = false;
	bool close_nough_y = false;

	if ( x_ < x - ( W / 2 ) - ( w / 2 ) - FAST_SPEED )
	{
		x_ += FAST_SPEED;
	}
	else if ( x_ > x - ( W / 2 ) - ( w / 2 ) + FAST_SPEED )
	{
		x_ -= FAST_SPEED;
	}
	else
	{
		close_nough_x = true;
	}

	if ( y_ < y - ( H / 2 ) - ( h / 2 ) - FAST_SPEED )
	{
		y_ += FAST_SPEED;
	}
	else if ( y_ > y - ( H / 2 ) - ( h / 2 ) + FAST_SPEED )
	{
		y_ -= FAST_SPEED;
	}
	else
	{
		close_nough_y = true;
	}
	
	return close_nough_x && close_nough_y;
};

void OWCamera::move( int map_w, int map_h )
{
	if ( Input::held( Input::Action::RUN) )
	{
		speed_ = FAST_SPEED;
	}
	else
	{
		speed_ = NORMAL_SPEED;
	}

	if ( Input::held( Input::Action::MOVE_LEFT ) )
	{
		x_ -= speed_;
	}
	else if ( Input::held( Input::Action::MOVE_RIGHT ) )
	{
		x_ += speed_;
	}

	if ( Input::held( Input::Action::MOVE_UP ) )
	{
		y_ -= speed_;
	}
	else if ( Input::held( Input::Action::MOVE_DOWN ) )
	{
		y_ += speed_;
	}
	
	bounds( map_w, map_h );
};

void OWCamera::bounds( int map_w, int map_h )
{
	if ( x_ < 0 )
	{
		x_ = 0;
	}
	else if ( x_ + W > map_w )
	{
		x_ = map_w - W;
	}
	
	if ( y_ < 0 )
	{
		y_ = 0;
	}
	else if ( y_ + H > map_h )
	{
		y_ = map_h - H;
	}
};