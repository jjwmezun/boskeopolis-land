#include "camera.hpp"
#include "input.hpp"
#include "map.hpp"
#include "sprite.hpp"

static constexpr int SHAKE_MAX = 8;
static constexpr int SCROLL_SPEED = 8;

void Camera::scroll( const Map& m )
{
	switch ( scroll_dir_ )
	{
		case ( Direction::Simple::UP ):
		{
			if ( y_ > position_before_scroll_ - height_ && y_ > 0 )
			{
				moveUp( SCROLL_SPEED );
			}
			else
			{
				scroll_dir_ = Direction::Simple::__NULL;
			}
		}
		break;

		case ( Direction::Simple::RIGHT ):
		{
			if ( x_ < position_before_scroll_ + width_ && screenRight() < m.widthPixels() )
			{
				moveRight( SCROLL_SPEED );
			}
			else
			{
				scroll_dir_ = Direction::Simple::__NULL;
			}
		}
		break;

		case ( Direction::Simple::DOWN ):
		{
			if ( y_ < position_before_scroll_ + height_ && screenBottom() < m.heightPixels() )
			{
				moveDown( SCROLL_SPEED );
			}
			else
			{
				scroll_dir_ = Direction::Simple::__NULL;
			}
		}
		break;

		case ( Direction::Simple::LEFT ):
		{
			if ( x_ > position_before_scroll_ - width_ && x_ > 0 )
			{
				moveLeft( SCROLL_SPEED );
			}
			else
			{
				scroll_dir_ = Direction::Simple::__NULL;
			}
		}
		break;
	}
};

void Camera::adjust( const Sprite& o, const Map& m )
{
	// Temporarily undo shake before updating position.
	x_ -= shake_amount_;
	setPrev();
	move( o, m.camera_type_ );
	contain( m );
	updateShaking();
};

void Camera::move( const Sprite& o, const Type type )
{
	switch( type )
	{
		case ( Type::NORMAL ):
		{
			moveXNormal( o );
			moveYNormal( o );
		}
		break;

		case ( Type::CENTER_X ):
		{
			moveXCenter( o );
			moveYNormal( o );
		}
		break;

		case ( Type::CENTER_Y ):
		{
			moveXNormal( o );
			moveYCenter( o );
		}
		break;

		case ( Type::CENTER_BOTH ):
		{
			moveXCenter( o );
			moveYCenter( o );
		}
		break;

		case ( Type::SCROLL_LOCK ):
		{
			if ( scroll_dir_ == Direction::Simple::__NULL )
			{
				if ( Input::held( Input::Action::MOVE_UP ) && o.yPixels() <= y_ && y_ > 0 )
				{
					position_before_scroll_ = y_;
					scroll_dir_ = Direction::Simple::UP;
				}
				else if ( Input::held( Input::Action::MOVE_RIGHT ) &&  o.rightPixels() >= screenRight() )
				{
					position_before_scroll_ = x_;
					scroll_dir_ = Direction::Simple::RIGHT;
				}
				else if ( Input::held( Input::Action::MOVE_DOWN ) &&  o.bottomPixels() >= screenBottom() )
				{
					position_before_scroll_ = y_;
					scroll_dir_ = Direction::Simple::DOWN;
				}
				else if ( Input::held( Input::Action::MOVE_LEFT ) && o.xPixels() <= x_ )
				{
					position_before_scroll_ = x_;
					scroll_dir_ = Direction::Simple::LEFT;
				}
			}
		}
		break;

		case ( Type::PRIORITIZE_TOP ):
		{
			moveXNormal( o );

			if ( o.yPixels() < boundaryCenterY() )
			{
				y_ -= boundaryCenterY() - o.yPixels();
			}

			if ( o. bottomPixels() > boundaryBottom() )
			{
				y_ += o. bottomPixels() - boundaryBottom();
			}
		}
		break;
	}
};

void Camera::adjustCart( const Sprite& o, const Map& m )
{
	setPrev();
	move( o, ( m.camera_type_ == Type::SCROLL_LOCK ) ? Type::SCROLL_LOCK : Type::CENTER_BOTH );
	contain( m );
};

void Camera::setPrev()
{
	prev_x_ = x_;
	prev_y_ = y_;
};

void Camera::contain( const Map& m )
{

	if ( !m.scrollLoop() )
	{
		if ( x_ < 0)
		{
			x_ = 0;
		}
		if ( x_ > m.widthPixels() - width_ - 0 )
		{
			x_ = m.widthPixels()  - width_ - 0;
		}
	}

	if ( y_ < 0 )
	{
		y_ = 0;
	}
	if ( y_ > m.heightPixels() - container_height_ - 0 )
	{
		y_ = m.heightPixels() - container_height_ - 0;
	}

	if ( m.top_limit_ > -1 )
	{
		if ( m.top_limit_ > y_ )
		{
			y_ = m.top_limit_;
		}
	}

	if ( m.bottom_limit_ > -1 )
	{
		if ( m.heightPixels() - m.bottom_limit_ < y_ + container_height_ )
		{
			y_ = m.heightPixels() - m.bottom_limit_ - container_height_;
		}
	}

	if ( m.left_limit_ > -1 )
	{
		if ( m.left_limit_ > x_ )
		{
			x_ = m.left_limit_;
		}
	}

	if ( m.right_limit_ > -1 )
	{
		if ( m.widthPixels() - m.right_limit_ < right() )
		{
			x_ = m.widthPixels() - m.right_limit_ - width_;
		}
	}

	if ( x() != prevX() || y() != prevY() )
	{
		changed_ = true;
	}
}

bool Camera::onscreen( sdl2::SDLRect r, int padding ) const
{
	return !offscreen( r, padding );
};

bool Camera::onscreenPixels( sdl2::SDLRect r, int padding ) const
{
	return !offscreen( Unit::PixelsToSubPixels( r ), padding );
};

bool Camera::offscreen( sdl2::SDLRect r, int padding, Direction::Simple dir ) const
{
	switch( dir )
	{
		case ( Direction::Simple::UP ):
			return offscreenTop( r, padding );
		break;
		case ( Direction::Simple::RIGHT ):
			return offscreenRight( r, padding );
		break;
		case ( Direction::Simple::DOWN ):
			return offscreenBottom( r, padding );
		break;
		case ( Direction::Simple::LEFT ):
			return offscreenLeft( r, padding );
		break;
		default:
			return offscreenTop( r, padding ) || offscreenRight( r, padding ) || offscreenBottom( r, padding ) || offscreenLeft( r, padding );
		break;
	}
};

void Camera::moveDown( int amount )
{
	y_ += amount;
	changed_ = true;
};

void Camera::moveUp( int amount )
{
	y_ -= amount;
	changed_ = true;
};

void Camera::moveLeft( int amount )
{
	x_ -= amount;
	changed_ = true;
};

void Camera::moveRight( int amount )
{
	x_ += amount;
	changed_ = true;
};

void Camera::setPosition( int x, int y )
{
	x_ = x;
	y_ = y;
	changed_ = true;
};

void Camera::startShaking()
{
	shake_dir_ = Direction::Horizontal::LEFT;
};

void Camera::moveXNormal( const Sprite& o )
{
	if ( o.xPixels() < boundaryLeft() )
	{
		x_ -= boundaryLeft() - o.xPixels();
	}

	if ( o.rightPixels() > boundaryRight() )
	{
		x_ += o.rightPixels() - boundaryRight();
	}
};

void Camera::moveYNormal( const Sprite& o )
{
	const int top_boundary = ( !o.onGround() ) ? boundaryTopWeaker() : boundaryTop();

	if ( o.yPixels() < boundaryTop() )
	{
		y_ -= boundaryTop() - o.yPixels();
	}

	if ( o. bottomPixels() > boundaryBottom() )
	{
		y_ += o. bottomPixels() - boundaryBottom();
	}
};

void Camera::moveXCenter( const Sprite& o )
{
	if ( o.centerXPixels() != boundaryCenterX() )
	{
		x_ = o.centerXPixels() - ( int )( std::round( ( double )( width_ ) / 2.0 ) );
	}
};

void Camera::moveYCenter( const Sprite& o )
{
	if ( o.centerYPixels() != boundaryCenterY() )
	{
		y_ = o.centerYPixels() - ( int )( std::round( ( double )( height_ ) / 2.0 ) );
	}
};

void Camera::updateShaking()
{
	switch ( shake_dir_ )
	{
		case ( Direction::Horizontal::LEFT ):
		{
			--shake_amount_;
			if ( shake_amount_ <= -SHAKE_MAX )
			{
				shake_dir_ = Direction::Horizontal::RIGHT;
			}
		}
		break;

		case ( Direction::Horizontal::RIGHT ):
		{
			++shake_amount_;
			if ( shake_amount_ >= SHAKE_MAX )
			{
				shake_dir_ = Direction::Horizontal::LEFT;
			}
		}
		break;
	}
	x_ += shake_amount_;
};
