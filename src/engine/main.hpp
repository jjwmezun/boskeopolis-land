class Palette;

#include "game_state.hpp"
#include <memory>
#include <string>
#include "text.hpp"

namespace Main
{
	bool running();
	void execute();
	void quit();
	void end();

	void changeState( std::unique_ptr<GameState> state );
	void pushState( std::unique_ptr<GameState> state );
	void popState();

	void tempMessage( std::string message, Palette&& palette, Text::FontColor text_color );

	bool nextFrame( int interval = 8, int duration = 1 );
	int frame();
	bool nextStateFrame( int interval = 8, int duration = 1 );
	int stateFrame();

	std::string resourcePath();
	std::string pathDivider();
	std::string savePath();

	bool savingAllowed();
};