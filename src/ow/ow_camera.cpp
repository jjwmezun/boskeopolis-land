#include "input.hpp"
#include "ow_camera.hpp"

static constexpr int NORMAL_SPEED = 5;
static constexpr int FAST_SPEED = 10;

void OWCamera::adjust( const sdl2::SDLRect& target )
{
	if ( target.x < leftBoundary() )
	{
		coords_.x -= leftBoundary() - target.x;
	}
	else if ( target.right() > rightBoundary() )
	{
		coords_.x += target.right() - rightBoundary();
	}

	if ( target.y < topBoundary() )
	{
		coords_.y -= topBoundary() - target.y;
	}
	else if ( target.bottom() > bottomBoundary() )
	{
		coords_.y += target.bottom() - bottomBoundary();
	}

	keepInBounds();
};


void OWCamera::center( const Point& target )
{
	coords_.x = target.x - ( int )( ( double )( WIDTH ) / 2.0 );
	coords_.y = target.y - ( int )( ( double )( HEIGHT ) / 2.0 );
	keepInBounds();
};

bool OWCamera::moveAutomaticallyToTarget( const Point& target )
{
	bool close_nough_x = false;
	bool close_nough_y = false;

	const int center_x = target.x - ( int )( ( double )( WIDTH ) / 2.0 );
	if ( coords_.x < center_x - FAST_SPEED )
	{
		coords_.x += FAST_SPEED;
	}
	else if ( coords_.x > center_x + FAST_SPEED )
	{
		coords_.x -= FAST_SPEED;
	}
	else
	{
		close_nough_x = true;
	}

	const int center_y = target.y - ( int )( ( double )( HEIGHT ) / 2.0 );
	if ( coords_.y < center_y - FAST_SPEED )
	{
		coords_.y += FAST_SPEED;
	}
	else if ( coords_.y > center_y + FAST_SPEED )
	{
		coords_.y -= FAST_SPEED;
	}
	else
	{
		close_nough_y = true;
	}
	
	return close_nough_x && close_nough_y;
};

void OWCamera::move()
{
	const int speed = ( Input::held( Input::Action::RUN) ) ? FAST_SPEED : NORMAL_SPEED;
	if ( Input::held( Input::Action::MOVE_LEFT ) )
	{
		coords_.x -= speed;
	}
	else if ( Input::held( Input::Action::MOVE_RIGHT ) )
	{
		coords_.x += speed;
	}

	if ( Input::held( Input::Action::MOVE_UP ) )
	{
		coords_.y -= speed;
	}
	else if ( Input::held( Input::Action::MOVE_DOWN ) )
	{
		coords_.y += speed;
	}
	
	 keepInBounds();
};

void OWCamera::keepInBounds()
{
	if ( coords_.x < 0 )
	{
		coords_.x = 0;
	}
	else if ( coords_.x + WIDTH > bounds_w_ )
	{
		coords_.x = bounds_w_ - WIDTH;
	}
	
	if ( coords_.y < 0 )
	{
		coords_.y = 0;
	}
	else if ( coords_.y + HEIGHT > bounds_h_ )
	{
		coords_.y = bounds_h_ - HEIGHT;
	}
};