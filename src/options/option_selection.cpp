#include "audio.hpp"
#include "option_selection.hpp"

OptionSelection::OptionSelection( int max, int not_allowed, int current )
:
	max_ ( max ),
	selection_ ( current ),
	previous_selection_ ( -1 ),
	timer_ ( 0 ),
	not_allowed_ ( not_allowed )
{};

int OptionSelection::selection() const { return selection_; };
int OptionSelection::previousSelection() const { return previous_selection_; };

void OptionSelection::update()
{
	if ( testInput( Input::Action::MOVE_UP ) )
	{
		selectUp();
		Audio::playSound( Audio::SoundType::SELECT );
		timer_ = 16;
	}
	else if ( testInput( Input::Action::MOVE_DOWN ) )
	{
		selectDown();
		Audio::playSound( Audio::SoundType::SELECT );
		timer_ = 16;
	}
	else
	{
		--timer_;
	}
};

void OptionSelection::selectUp()
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

void OptionSelection::selectDown()
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

bool OptionSelection::testInput( Input::Action input ) const
{
	return Input::pressed( input ) || ( Input::held( input ) && timer_ <= 0 );
}

void OptionSelection::setNotAllowed( int not_allowed )
{
	not_allowed_ = not_allowed;
};

void OptionSelection::setSelect( int value )
{
	selection_ = value;
};
