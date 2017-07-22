#include "collision.h"
#include <iostream>
#include "input.h"
#include "ow_camera.hpp"
#include "ow_hero.hpp"
#include "ow_level.hpp"

OWHero::OWHero( int x, int y )
:
	gfx_ ( "sprites/autumn_overworld.png", { std::make_pair( 0, 0 ), std::make_pair( 15, 0 ) }, false, false, 0, false, -2, -1, 3, 2 ),
	x_ ( x ),
	y_ ( y ),
	speed_ ( 2 )
{};

void OWHero::update( const Input& input )
{
	if ( input.held( Input::Action::RUN ) )
	{
		speed_ = 4;
	}
	else
	{
		speed_ = 2;
	}
	
	if ( input.held( Input::Action::MOVE_RIGHT ) )
	{
		x_ += speed_;
	}
	else if ( input.held( Input::Action::MOVE_LEFT ) )
	{
		x_ -= speed_;
	}

	if ( input.held( Input::Action::MOVE_DOWN ) )
	{
		y_ += speed_;
	}
	else if ( input.held( Input::Action::MOVE_UP ) )
	{
		y_ -= speed_;
	}
	
	gfx_.update();
};

void OWHero::render( Graphics& graphics, const OWCamera& camera )
{
	gfx_.render( graphics, camera.relative( coords() ) );
};

int OWHero::x() const { return x_; };
int OWHero::y() const { return y_; };
int OWHero::right() const { return x_ + W; };
int OWHero::bottom() const { return y_ + H; };

void OWHero::collideStop( const Collision& collision )
{
	y_ += collision.overlapYTop();
	y_ -= collision.overlapYBottom();
	x_ += collision.overlapXLeft();
	x_ -= collision.overlapXRight();
};

sdl2::SDLRect OWHero::coords() const
{
	return
	{
		x_,
		y_,
		W,
		H
	};
};

void OWHero::placeOnLv( const OWLevel& lv )
{
	x_ = lv.x();
	y_ = lv.y();
};