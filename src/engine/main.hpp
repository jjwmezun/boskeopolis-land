#pragma once

class Palette;

#include "game_state.hpp"
#include <memory>
#include <string>
#include "text.hpp"

namespace Main
{
	enum class TransitionState
	{
		__NULL,
		FADE_OUT,
		FADE_IN
	};

	void quit();

	void changeState( std::unique_ptr<GameState> state );
	void pushState( std::unique_ptr<GameState> state, bool transition = false );
	void popState( bool transition = false );

	bool nextFrame( int interval = 8, int duration = 1 );
	int frame();
	bool nextStateFrame( int interval = 8, int duration = 1 );
	int stateFrame();

	std::string resourcePath();
	std::string pathDivider();
	std::string savePath();

	bool nosave();
	bool noharm();
	bool stillFading();
	void startFadeOut();
	TransitionState transitionState();
	const Palette& getPalette();
};
