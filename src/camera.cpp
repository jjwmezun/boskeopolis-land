

// Name
//===================================
//
// Camera
//


// DEPENDENCIES
//===================================

    #include "camera.hpp"
    #include "map.hpp"
    #include "sprite.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    bool Camera::changed() const
    {
		return changed_;
    };
	
	void Camera::update()
	{
		changed_ = false;
	};

    void Camera::adjust( Sprite& o, Map& m )
    {
        setPrev();

        switch( m.cameraXPriority() )
        {
            case ( XPriority::CENTER ):

                if ( o.centerXPixels() != boundaryCenterX() )
                {
                    x_ = o.centerXPixels() - round( widthPixels() / 2 );
                }

            break;

            case ( XPriority::__NULL ):

                if ( o.xPixels() < boundaryLeft() )
                {
                    x_ -= boundaryLeft() - o.xPixels();
                }

                if ( o.rightPixels() > boundaryRight() )
                {
                    x_ += o.rightPixels() - boundaryRight();
                }

            break;
        }

        switch( m.cameraYPriority() )
        {
            case ( YPriority::CENTER ):

                if ( o.centerYPixels() != boundaryCenterY() )
                {
                    y_ = o.centerYPixels() - round( heightPixels() / 2 );
                }

            break;

            case ( YPriority::__NULL ):


                const int top_boundary = ( !o.onGround() ) ? boundaryTopWeaker() : boundaryTop();

                if ( o.yPixels() < boundaryTop() )
                {
                    y_ -= boundaryTop() - o.yPixels();
                }

                if ( o. bottomPixels() > boundaryBottom() )
                {
                    y_ += o. bottomPixels() - boundaryBottom();
                }

            break;
        }

        contain( m );
    };

    void Camera::adjustCart( Sprite& o, Map& m )
    {
        setPrev();

        if ( o.centerXPixels() != boundaryCenterX() )
            x_ = o.centerXPixels() - round( widthPixels() / 2 );

        if ( o.centerYPixels() != boundaryCenterY() )
            y_ = o.centerYPixels() - round( heightPixels() / 2 );

        contain( m );
    };

    void Camera::setPrev()
    {
        prev_x_ = x_;
        prev_y_ = y_;
    };

    void Camera::contain( Map& m )
    {

        if ( !m.scrollLoop() )
        {
            if ( x_ < 0)
            {
                x_ = 0;
            }
            if ( x_ > m.widthPixels() - widthPixels() - 0 )
            {
                x_ = m.widthPixels()  - widthPixels() - 0;
            }
        }

        if ( y_ < 0 )
        {
            y_ = 0;
        }
        if ( y_ > m.heightPixels() - heightPixels() - 0 )
        {
            y_ = m.heightPixels() - heightPixels() - 0;
        }

        if ( m.topLimit() > -1 )
        {
            if ( m.topLimit() > y_ )
            {
                y_ = m.topLimit();
            }
        }

        if ( m.leftLimit() > -1 )
        {
            if ( m.leftLimit() > x_ )
            {
                x_ = m.leftLimit();
            }
        }

        if ( m.rightLimit() > -1 )
        {
            if ( m.widthPixels() - m.rightLimit() < right() )
            {
                x_ = m.widthPixels() - m.rightLimit() - widthPixels();
            }
        }

        if ( x() != prevX() || y() != prevY() )
            changed_ = true;
    }

    bool Camera::onscreen( const sdl::rect& r, int padding ) const
    {
        return !offscreen( r, padding );
    };

    bool Camera::onscreenPixels( const sdl::rect& r, int padding ) const
    {
        sdl::rect new_r = Unit::PixelsToSubPixels( r );
        return !offscreen( new_r, padding );
    };

    bool Camera::offscreen( const sdl::rect& r, int padding, Direction::Simple dir ) const
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
