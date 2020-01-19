#include "input.hpp"
#include "ow_camera.hpp"
#include "ow_hero.hpp"

static constexpr int WALK_SPEED = 2;
static constexpr int RUN_SPEED = WALK_SPEED * 2;
static constexpr int GRAPHICS_WIDTH = 16;
static constexpr int GRAPHICS_HEIGHT = 16;
static constexpr int GRAPHICS_HALF_WIDTH = ( int )( ( double )( GRAPHICS_WIDTH ) / 2.0 );
static constexpr int GRAPHICS_HALF_HEIGHT = ( int )( ( double )( GRAPHICS_WIDTH ) / 2.0 );

static constexpr int calculateGraphicsXFromPositionX( int position_x )
{
	return position_x - GRAPHICS_HALF_WIDTH;
};

static constexpr int calculateGraphicsYFromPositionY( int position_y )
{
	return position_y - GRAPHICS_HALF_HEIGHT;
};

OWHero::OWHero( int x, int y )
:
	position_ ({ x, y }),
	graphics_
	(
		"sprites/autumn-overworld.png",
		{ 0, 0, GRAPHICS_WIDTH, GRAPHICS_HEIGHT },
		{ calculateGraphicsXFromPositionX( x ), calculateGraphicsYFromPositionY( y ), GRAPHICS_WIDTH, GRAPHICS_HEIGHT } ),
	absolute_graphics_box_ ( graphics_.dest_ )
{};

void OWHero::update( const sdl2::SDLRect& bounds )
{
	const int speed = ( Input::held( Input::Action::RUN ) ) ? RUN_SPEED : WALK_SPEED;
	if ( Input::held( Input::Action::MOVE_RIGHT ) )
	{
		position_.x += speed;
		updateGraphicsX( bounds );
	}
	else if ( Input::held( Input::Action::MOVE_LEFT ) )
	{
		position_.x -= speed;
		updateGraphicsX( bounds );
	}

	if ( Input::held( Input::Action::MOVE_DOWN ) )
	{
		position_.y += speed;
		updateGraphicsY( bounds );
	}
	else if ( Input::held( Input::Action::MOVE_UP ) )
	{
		position_.y -= speed;
		updateGraphicsY( bounds );
	}
	keepInBounds( bounds );
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

const Point& OWHero::getPosition() const
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
		updateGraphicsX( bounds );
	}
	else if ( absolute_graphics_box_.right() > bounds.right() )
	{
		position_.x = bounds.right() - GRAPHICS_HALF_WIDTH;
		updateGraphicsX( bounds );
	}

	if ( absolute_graphics_box_.y < 0 )
	{
		position_.y = GRAPHICS_HALF_HEIGHT;
		updateGraphicsY( bounds );
	}
	else if ( absolute_graphics_box_.bottom() > bounds.bottom() )
	{
		position_.y = bounds.bottom() - GRAPHICS_HALF_HEIGHT;
		updateGraphicsY( bounds );
	}
}