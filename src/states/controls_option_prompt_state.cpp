#include "controls_option_prompt_state.hpp"
#include "main.hpp"
#include "render.hpp"

static constexpr int WIDTH = 17 * Text::CHAR_SIZE_PIXELS;
static constexpr int HEIGHT = 5 * Text::CHAR_SIZE_PIXELS;
static constexpr int X = ( Unit::WINDOW_WIDTH_PIXELS - WIDTH ) / 2 - 1;
static constexpr int Y = ( Unit::WINDOW_HEIGHT_PIXELS - HEIGHT ) / 2 - 1;

ControlsOptionPromptState::ControlsOptionPromptState( Input::Action action )
:
	GameState( StateID::OPTIONS_STATE, { "Mountain Red", 2 } ),
	text_ ( "Press Any Key", 0, 0, Text::FontColor::WHITE, Text::FontAlign::CENTER, Text::FontColor::__NULL, true ),
	bg_ ( X, Y, WIDTH, HEIGHT ),
	shadow_ ( X + 2, Y + 2, WIDTH, HEIGHT ),
	action_ ( action )
{
	Input::setKeycodeChangeStart( action_ );
};

ControlsOptionPromptState::~ControlsOptionPromptState() {};

void ControlsOptionPromptState::stateUpdate()
{
	if ( Input::testKeycodeChangeDone() )
	{
		Main::popState();
	}
};

void ControlsOptionPromptState::stateRender()
{
	Render::renderRect( shadow_, 6 );
	Render::renderRect( bg_, 5 );
	text_.render();
};

void ControlsOptionPromptState::init() {};
