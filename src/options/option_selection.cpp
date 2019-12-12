#include "audio.hpp"
#include "option_selection.hpp"

OptionSelection::OptionSelection( int current, int not_allowed )
:
	selection_ ( current ),
	previous_selection_ ( -1 ),
	timer_ ( 0 ),
	not_allowed_ ( not_allowed )
{};

OptionSelection::~OptionSelection() {};

int OptionSelection::selection() const { return selection_; };
int OptionSelection::previousSelection() const { return previous_selection_; };

void OptionSelection::update()
{
	if ( !customUpdate() )
	{
		playSound();
		timer_ = 16;
	}
	else
	{
		--timer_;
	}
};

bool OptionSelection::testInput( Input::Action input ) const
{
	return Input::pressed( input ) || ( Input::held( input ) && timer_ <= 0 );
}

void OptionSelection::setSelect( int value )
{
	selection_ = value;
};

void OptionSelection::playSound() const
{
	Audio::playSound( Audio::SoundType::SELECT );
}

void OptionSelection::setNotAllowed( int not_allowed )
{
	not_allowed_ = not_allowed;
	if ( selection_ == not_allowed_ )
	{
		shoveForward();
	}
};