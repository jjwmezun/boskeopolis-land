#pragma once

#include <SDL2/SDL.h>

namespace Input
{
	enum Action
	{
		CONFIRM,
		CANCEL,
		MENU,
		JUMP,
		RUN,
		MOVE_UP,
		MOVE_RIGHT,
		MOVE_DOWN,
		MOVE_LEFT,
		CAMERA_LEFT,
		CAMERA_RIGHT,
		CAMERA_UP,
		CAMERA_DOWN,
		ESCAPE
	};
	static constexpr int NUM_O_ACTIONS = ( int )( Action::ESCAPE + 1 );

	bool pressed( Action action );
	bool pressedMain();
	bool released( Action action );
	bool held( Action action );

	void update();
	void reset();
	void keyPress( SDL_Keycode key );
	void keyRelease( SDL_Keycode key );
	void keyHold( SDL_Keycode key );
};