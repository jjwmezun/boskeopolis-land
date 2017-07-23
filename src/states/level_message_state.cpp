#include "game.h"
#include "input.h"
#include "level_message_state.h"
#include "render.h"

constexpr sdl2::SDLRect LevelMessageState::backdrop_;

LevelMessageState::LevelMessageState( const Palette& palette, std::string message )
:
	GameState( StateID::PAUSE_STATE, palette ),
	message_
	(
		Text
		(
			message,
			TEXT_X,
			TEXT_Y,
			Text::FontShade::WHITE,
			Text::FontAlign::LEFT,
			false,
			LINE_LIMIT 
		)
	)
{};

LevelMessageState::~LevelMessageState() {};

void LevelMessageState::update( Game& game, const Input& input )
{
	if ( input.pressedMain() )
	{
		game.popState();
	}
};

void LevelMessageState::stateRender()
{
	Render::renderRect( backdrop_, 6 );
	message_.render();
};

void LevelMessageState::init( Game& game ) {};