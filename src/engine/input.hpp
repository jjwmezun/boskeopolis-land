#pragma once

#include <SDL2/SDL.h>
#include <string>

#define USE_CONTROLLER

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
		CAMERA_DOWN
	};
	static constexpr int NUM_O_ACTIONS = ( int )( Action::CAMERA_DOWN + 1 );

	bool pressed( Action action );
	bool pressedMain();
	bool released( Action action );
	bool held( Action action );
	bool movingCharacter();
	bool movingCharacterNoHold();
	std::string getActionName( Action action );
	int calculateMaxActionNameLength();
	std::string getKeyName( Action action );
	std::string getButtonName( Action action );

	void init();
	void close();
	void update();
	void reset();
	void renderQuitText();
	void setKeycodeChangeStart( Action action );
	bool testKeycodeChangeDone();

	void keyPress( SDL_Keycode key );
	void keyRelease( SDL_Keycode key );
	void keyHold( SDL_Keycode key );
	void buttonPress( Uint8 button );
	void buttonRelease( Uint8 button );
	void buttonHold( Uint8 button );
	void axis( const SDL_JoyAxisEvent& axis_event );
	bool exitButtonHeldLongEnough();
};
