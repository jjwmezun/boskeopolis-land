#include "input.h"

Input::Input()
{
	reset();
};

void Input::reset()
{
	resetList( keys_pressed_ );
	resetList( keys_released_ );
	resetList( keys_held_ );
};

void Input::update()
{
	resetList( keys_pressed_ );
	resetList( keys_released_ );
};

void Input::resetList( std::map<SDL_Keycode, bool>& list )
{
	for ( int i = 0; i < NUM_O_ACTIONS; ++i )
	{
		list[ buttons_.at( ( Action )i ) ] = false;
	}
};

bool Input::pressed( Action action ) const
{
	return keys_pressed_.at( ( buttons_.at( action ) ) );
};

bool Input::pressedMain() const
{
	return pressed( Action::CONFIRM ) || pressed( Action::CANCEL ) || pressed( Action::MENU );
};

bool Input::released( Action action ) const
{
	return keys_released_.at( buttons_.at( action ) );
};

bool Input::held( Action action ) const
{
	return keys_held_.at( buttons_.at( action ) );
};

void Input::keyPress( SDL_Keycode key )
{
	if ( !keys_pressed_before_released_[ key ] )
	{
		keys_pressed_[ key ] = true;
		keys_pressed_before_released_[ key ] = true;
	}
};

void Input::keyRelease( SDL_Keycode key )
{
	keys_released_[ key ] = true;
	keys_pressed_[ key ] = false;
	keys_held_[ key ] = false;
	keys_pressed_before_released_[ key ] = false;
};

void Input::keyHold( SDL_Keycode key )
{
	keys_held_[ key ] = true;
};