#include "main.hpp"
#include "input.hpp"
#include "level_message_state.hpp"
#include "render.hpp"

constexpr sdl::rect LevelMessageState::backdrop_;

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

void LevelMessageState::update()
{
	if ( Input::pressedMain() )
	{
		Main::popState();
	}
};

void LevelMessageState::stateRender()
{
	Render::renderRect( backdrop_, 6 );
	message_.render();
};

void LevelMessageState::init() {};