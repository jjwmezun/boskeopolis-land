#include "controls_option_prompt_state.hpp"
#include "input.hpp"
#include "main.hpp"
#include "render.hpp"

static constexpr int WIDTH = 17 * Text::CHAR_SIZE_PIXELS;
static constexpr int HEIGHT = 5 * Text::CHAR_SIZE_PIXELS;
static constexpr int X = ( Unit::WINDOW_WIDTH_PIXELS - WIDTH ) / 2;
static constexpr int Y = ( Unit::WINDOW_HEIGHT_PIXELS - HEIGHT ) / 2;

ControlsOptionPromptState::ControlsOptionPromptState()
:
	GameState( StateID::OPTIONS_STATE, { "Mountain Red", 2 } ),
	text_ ( "Press Any Key", 0, 0, Text::FontColor::WHITE, Text::FontAlign::CENTER, Text::FontColor::__NULL, true ),
	bg_ ( X, Y, WIDTH, HEIGHT )
{};

ControlsOptionPromptState::~ControlsOptionPromptState() {};

void ControlsOptionPromptState::stateUpdate()
{
	if ( Input::pressed( Input::Action::CANCEL ) )
	{
		Main::popState();
	}
	else if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		Main::popState();
	}
};

void ControlsOptionPromptState::stateRender()
{
	Render::renderRect( bg_, 5 );
	text_.render();
};

void ControlsOptionPromptState::init() {};
