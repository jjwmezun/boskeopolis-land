#include "input.hpp"
#include "ow_camera.hpp"
#include "ow_hero.hpp"
#include "ow_tilemap.hpp"

static constexpr double WALK_ACCELERATION = 0.1;
static constexpr double RUN_ACCELERATION = WALK_ACCELERATION * 2.0;
static constexpr double WALK_TOP_SPEED = 2.0;
static constexpr double RUN_TOP_SPEED = WALK_TOP_SPEED * 2.0;
static constexpr double BOUNCE_BACK = -0.16;
static constexpr int GRAPHICS_WIDTH = 16;
static constexpr int GRAPHICS_HEIGHT = 16;
static constexpr int GRAPHICS_HALF_WIDTH = ( int )( ( double )( GRAPHICS_WIDTH ) / 2.0 );
static constexpr int GRAPHICS_HALF_HEIGHT = ( int )( ( double )( GRAPHICS_WIDTH ) / 2.0 );

static constexpr int calculateGraphicsXFromPositionX( double position_x )
{
	return ( int )( std::floor( position_x ) ) - GRAPHICS_HALF_WIDTH;
};

static constexpr int calculateGraphicsYFromPositionY( double position_y )
{
	return ( int )( std::floor( position_y ) ) - ( GRAPHICS_HEIGHT - 4 );
};

OWHero::OWHero( int x, int y )
:
	position_ ({ x, y }),
	graphics_
	(
		"sprites/autumn-overworld.png",
		{ 0, 0, GRAPHICS_WIDTH, GRAPHICS_HEIGHT },
		{ calculateGraphicsXFromPositionX( x ), calculateGraphicsYFromPositionY( y ), GRAPHICS_WIDTH, GRAPHICS_HEIGHT } ),
	absolute_graphics_box_ ( graphics_.dest_ ),
	x_speed_ ( 0.0 ),
	y_speed_ ( 0.0 ),
	animation_timer_ ()
{};

void OWHero::update( const OWTileMap& tilemap, const sdl2::SDLRect& bounds )
{
	const double acceleration_speed = ( Input::held( Input::Action::RUN ) ) ? RUN_ACCELERATION : WALK_ACCELERATION;
	const double top_speed = ( Input::held( Input::Action::RUN ) ) ? RUN_TOP_SPEED : WALK_TOP_SPEED;
	double x_acceleration = 0.0;
	double y_acceleration = 0.0;
	if ( Input::held( Input::Action::MOVE_RIGHT ) )
	{
		x_acceleration = acceleration_speed;
	}
	else if ( Input::held( Input::Action::MOVE_LEFT ) )
	{
		x_acceleration = -acceleration_speed;
	}

	if ( Input::held( Input::Action::MOVE_DOWN ) )
	{
		y_acceleration = acceleration_speed;
	}
	else if ( Input::held( Input::Action::MOVE_UP ) )
	{
		y_acceleration = -acceleration_speed;
	}

	x_speed_ += x_acceleration;
	if ( x_speed_ > top_speed )
	{
		x_speed_ = top_speed;
	}
	else if ( x_speed_ < -top_speed )
	{
		x_speed_ = -top_speed;
	}
	y_speed_ += y_acceleration;
	if ( y_speed_ > top_speed )
	{
		y_speed_ = top_speed;
	}
	else if ( y_speed_ < -top_speed )
	{
		y_speed_ = -top_speed;
	}

	if ( x_acceleration == 0.0 )
	{
		x_speed_ /= 1.25;
	}

	if ( y_acceleration == 0.0 )
	{
		y_speed_ /= 1.25;
	}

	const int v_tile_x = ( int )( std::floor( ( position_.x ) / 16.0 ) );
	const int v_tile_y = ( int )( std::floor( ( position_.y + y_speed_ ) / 16.0 ) );
	const int v_tile = v_tile_y * tilemap.width() + v_tile_x;
	if ( !tilemap.isSolid( v_tile ) )
	{
		position_.y += y_speed_;
	}
	else
	{
		y_speed_ *= BOUNCE_BACK;
	}

	const int h_tile_x = ( int )( std::floor( ( position_.x + x_speed_ ) / 16.0 ) );
	const int h_tile_y = ( int )( std::floor( ( position_.y ) / 16.0 ) );
	const int h_tile = h_tile_y * tilemap.width() + h_tile_x;
	if ( !tilemap.isSolid( h_tile ) )
	{
		position_.x += x_speed_;
	}
	else
	{
		x_speed_ *= BOUNCE_BACK;
	}

	keepInBounds( bounds );
	updateGraphics( bounds );
};

void OWHero::updateGraphics( const sdl2::SDLRect& bounds )
{
	updateGraphicsX( bounds );
	updateGraphicsY( bounds );
};

void OWHero::updateGraphicsX( const sdl2::SDLRect& bounds )
{
	absolute_graphics_box_.x = calculateGraphicsXFromPositionX( position_.x );
	graphics_.dest_.x = absolute_graphics_box_.x - bounds.x;
};

void OWHero::updateGraphicsY( const sdl2::SDLRect& bounds )
{
	absolute_graphics_box_.y = calculateGraphicsYFromPositionY( position_.y );
	graphics_.dest_.y = absolute_graphics_box_.y - bounds.y;
};

void OWHero::render( const OWCamera& camera )
{
	graphics_.render();
};

int OWHero::x() const { return position_.x; };
int OWHero::y() const { return position_.y; };

const DPoint& OWHero::getPosition() const
{
	return position_;
};

const sdl2::SDLRect& OWHero::getGraphicsBox() const
{
	return absolute_graphics_box_;
};

void OWHero::keepInBounds( const sdl2::SDLRect& bounds )
{
	if ( absolute_graphics_box_.x < 0 )
	{
		position_.x = GRAPHICS_HALF_WIDTH;
		x_speed_ *= BOUNCE_BACK;
		updateGraphicsX( bounds );
	}
	else if ( absolute_graphics_box_.right() > bounds.right() )
	{
		position_.x = bounds.right() - GRAPHICS_HALF_WIDTH;
		x_speed_ *= BOUNCE_BACK;
		updateGraphicsX( bounds );
	}

	if ( absolute_graphics_box_.y < 0 )
	{
		position_.y = GRAPHICS_HALF_HEIGHT;
		y_speed_ *= BOUNCE_BACK;
		updateGraphicsY( bounds );
	}
	else if ( absolute_graphics_box_.bottom() > bounds.bottom() )
	{
		position_.y = bounds.bottom() - GRAPHICS_HALF_HEIGHT;
		y_speed_ *= BOUNCE_BACK;
		updateGraphicsY( bounds );
	}
};

void OWHero::setPosition( int x, int y, const sdl2::SDLRect& bounds )
{
	position_.x = ( double )( x );
	position_.y = ( double )( y );
	updateGraphics( bounds );
};

void OWHero::updateAnimation()
{
	if ( animation_timer_.update() )
	{
		graphics_.src_.x = ( graphics_.src_.x == 0 ) ? 16 : 0;
	}
};

void OWHero::reset( int x, int y )
{
	x_speed_ = 0.0;
	y_speed_ = 0.0;
	animation_timer_.reset();
	graphics_.src_.x = 0;
	position_.x = x;
	position_.y = y;
};