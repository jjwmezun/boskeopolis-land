#include "input.hpp"
#include "mezun_math.hpp"
#include "option_selection_grid.hpp"

#include <iostream>

OptionSelectionGrid::OptionSelectionGrid( int rows, int cols, int max )
:
	OptionSelection( 0 ),
	rows_ ( rows ),
    cols_ ( cols ),
    x_ ( 0 ),
    y_ ( 0 ),
	max_ ( max )
{};

OptionSelectionGrid::~OptionSelectionGrid() {};

bool OptionSelectionGrid::customUpdate()
{
	if ( testInput( Input::Action::MOVE_UP ) )
	{
		selectUp();
		return true;
	}
	else if ( testInput( Input::Action::MOVE_DOWN ) )
	{
		selectDown();
		return true;
	}
	else if ( testInput( Input::Action::MOVE_LEFT ) )
	{
		selectLeft();
		return true;
	}
	else if ( testInput( Input::Action::MOVE_RIGHT ) )
	{
		selectRight();
		return true;
	}
	return false;
};

void OptionSelectionGrid::selectUp()
{
	previous_selection_ = selection_;

	do
	{
		--y_;
		if ( y_ < 0 )
		{
			y_ = rows_ - 1;
		}
        selection_ = mezun::nOfXY( x_, y_, cols_ );
	}
	while ( testSelectionIsInvalid() );
};

void OptionSelectionGrid::selectDown()
{
	previous_selection_ = selection_;

	do
	{
		++y_;
		if ( y_ >= rows_ )
		{
			y_ = 0;
		}
        selection_ = mezun::nOfXY( x_, y_, cols_ );
	}
	while ( testSelectionIsInvalid() );
};

void OptionSelectionGrid::selectLeft()
{
	previous_selection_ = selection_;

	do
	{
		--x_;
		if ( x_ < 0 )
		{
			x_ = cols_ - 1;
		}
        selection_ = mezun::nOfXY( x_, y_, cols_ );
	}
	while ( testSelectionIsInvalid() );
};

void OptionSelectionGrid::selectRight()
{
	previous_selection_ = selection_;

	do
	{
		++x_;
		if ( x_ >= cols_ )
		{
			x_ = 0;
		}
        selection_ = mezun::nOfXY( x_, y_, cols_ );
	}
	while ( testSelectionIsInvalid() );
};

void OptionSelectionGrid::shoveForward()
{
	++selection_;
	if ( selection_ > max_ )
	{
		selection_ = 0;
	}
	x_ = mezun::xOfN( selection_, cols_ );
	y_ = mezun::yOfN( selection_, cols_ );
};