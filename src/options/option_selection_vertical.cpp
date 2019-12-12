#include "input.hpp"
#include "option_selection_vertical.hpp"

OptionSelectionVertical::OptionSelectionVertical( int max, int not_allowed, int current )
:
	OptionSelection( current, not_allowed ),
	max_ ( max )
{};

OptionSelectionVertical::~OptionSelectionVertical() {};

bool OptionSelectionVertical::customUpdate()
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
	return false;
};

void OptionSelectionVertical::selectUp()
{
	previous_selection_ = selection_;

	do
	{
		--selection_;
		if ( selection_ < 0 )
		{
			selection_ = max_;
		}
	}
	while ( selection_ == not_allowed_ );
};

void OptionSelectionVertical::selectDown()
{
	previous_selection_ = selection_;

	do
	{
		++selection_;
		if ( selection_ > max_ )
		{
			selection_ = 0;
		}
	}
	while ( selection_ == not_allowed_ );
};

void OptionSelectionVertical::shoveForward()
{
	++selection_;
	if ( selection_ > max_ )
	{
		selection_ = 0;
	}
};