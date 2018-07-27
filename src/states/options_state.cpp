#include "input.hpp"
#include "main.hpp"
#include "options_state.hpp"
#include "render.hpp"

OptionsState::OptionsState()
:
	GameState( StateID::OPTIONS_STATE, { "Mountain Red", 2 }, false )
{};

OptionsState::~OptionsState() {};

void OptionsState::stateUpdate()
{
	if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		Main::popState();
	}
};

void OptionsState::stateRender()
{
	Render::colorCanvas( 2 );
};

void OptionsState::init()
{
};
