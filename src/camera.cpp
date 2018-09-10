#include "camera.hpp"
#include "input.hpp"
#include "map.hpp"
#include "sprite.hpp"

static constexpr int SHAKE_MAX = 8;
static constexpr int SCROLL_SPEED = 8;

bool Camera::changed() const
{
	return changed_;
};

void Camera::update()
{
	changed_ = false;
}

void Camera::scroll( const Map& m )
{
	switch ( scroll_dir_ )
	{
		case ( Direction::Simple::UP ):
		{
			if ( camera_.y_ > position_before_scroll_ - camera_.heightPixels() && camera_.y_ > 0 )
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
			if ( camera_.x_ < position_before_scroll_ + camera_.widthPixels() && camera_.right() < m.widthPixels() )
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
			if ( camera_.y_ < position_before_scroll_ + camera_.heightPixels() && camera_.bottom() < m.heightPixels() )
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
			if ( camera_.x_ > position_before_scroll_ - camera_.widthPixels() && camera_.x_ > 0 )
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
	camera_.x_ -= shake_amount_;
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
				if ( Input::held( Input::Action::MOVE_UP ) && o.yPixels() <= camera_.y_ && camera_.y_ > 0 )
				{
					position_before_scroll_ = camera_.y_;
					scroll_dir_ = Direction::Simple::UP;
				}
				else if ( Input::held( Input::Action::MOVE_RIGHT ) &&  o.rightPixels() >= camera_.right() )
				{
					position_before_scroll_ = camera_.x_;
					scroll_dir_ = Direction::Simple::RIGHT;
				}
				else if ( Input::held( Input::Action::MOVE_DOWN ) &&  o.bottomPixels() >= camera_.bottom() )
				{
					position_before_scroll_ = camera_.y_;
					scroll_dir_ = Direction::Simple::DOWN;
				}
				else if ( Input::held( Input::Action::MOVE_LEFT ) && o.xPixels() <= camera_.x_ )
				{
					position_before_scroll_ = camera_.x_;
					scroll_dir_ = Direction::Simple::LEFT;
				}
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
	prev_x_ = camera_.x_;
	prev_y_ = camera_.y_;
};

void Camera::contain( const Map& m )
{

	if ( !m.scrollLoop() )
	{
		if ( camera_.x_ < 0)
		{
			camera_.x_ = 0;
		}
		if ( camera_.x_ > m.widthPixels() - widthPixels() - 0 )
		{
			camera_.x_ = m.widthPixels()  - widthPixels() - 0;
		}
	}

	if ( camera_.y_ < 0 )
	{
		camera_.y_ = 0;
	}
	if ( camera_.y_ > m.heightPixels() - heightPixels() - 0 )
	{
		camera_.y_ = m.heightPixels() - heightPixels() - 0;
	}

	if ( m.top_limit_ > -1 )
	{
		if ( m.top_limit_ > camera_.y_ )
		{
			camera_.y_ = m.top_limit_;
		}
	}

	if ( m.bottom_limit_ > -1 )
	{
		if ( m.heightPixels() - m.bottom_limit_ < bottom() )
		{
			camera_.y_ = m.heightPixels() - m.bottom_limit_ - heightPixels();
		}
	}

	if ( m.left_limit_ > -1 )
	{
		if ( m.left_limit_ > camera_.x_ )
		{
			camera_.x_ = m.left_limit_;
		}
	}

	if ( m.right_limit_ > -1 )
	{
		if ( m.widthPixels() - m.right_limit_ < right() )
		{
			camera_.x_ = m.widthPixels() - m.right_limit_ - widthPixels();
		}
	}

	if ( x() != prevX() || y() != prevY() )
	{
		changed_ = true;
	}
}

bool Camera::onscreen( const sdl2::SDLRect& r, int padding ) const
{
	return camera_.onscreen( r, padding );
};

bool Camera::onscreenPixels( const sdl2::SDLRect& r, int padding ) const
{
	return camera_.onscreenPixels( r, padding );
};

bool Camera::offscreen( const sdl2::SDLRect& r, int padding, Direction::Simple dir ) const
{
	return camera_.offscreen( r, padding, dir );
};

void Camera::moveDown( int amount )
{
	camera_.y_ += amount;
	changed_ = true;
};

void Camera::moveUp( int amount )
{
	camera_.y_ -= amount;
	changed_ = true;
};

void Camera::moveLeft( int amount )
{
	camera_.x_ -= amount;
	changed_ = true;
};

void Camera::moveRight( int amount )
{
	camera_.x_ += amount;
	changed_ = true;
};

void Camera::setPosition( int x, int y )
{
	camera_.x_ = x;
	camera_.y_ = y;
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
		camera_.x_ -= boundaryLeft() - o.xPixels();
	}

	if ( o.rightPixels() > boundaryRight() )
	{
		camera_.x_ += o.rightPixels() - boundaryRight();
	}
};

void Camera::moveYNormal( const Sprite& o )
{
	const int top_boundary = ( !o.onGround() ) ? boundaryTopWeaker() : boundaryTop();

	if ( o.yPixels() < boundaryTop() )
	{
		camera_.y_ -= boundaryTop() - o.yPixels();
	}

	if ( o. bottomPixels() > boundaryBottom() )
	{
		camera_.y_ += o. bottomPixels() - boundaryBottom();
	}
};

void Camera::moveXCenter( const Sprite& o )
{
	if ( o.centerXPixels() != boundaryCenterX() )
	{
		camera_.x_ = o.centerXPixels() - round( widthPixels() / 2 );
	}
};

void Camera::moveYCenter( const Sprite& o )
{
	if ( o.centerYPixels() != boundaryCenterY() )
	{
		camera_.y_ = o.centerYPixels() - round( heightPixels() / 2 );
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
	camera_.x_ += shake_amount_;
};

bool Camera::testPause() const
{
	return scroll_dir_ != Direction::Simple::__NULL;
};
