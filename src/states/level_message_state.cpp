#include "main.hpp"
#include "input.hpp"
#include "level_message_state.hpp"
#include "render.hpp"

static constexpr int TEXT_X = 32;
static constexpr int TEXT_Y = 32;
static constexpr int MESSAGE_BOX_PADDING = 2;
static constexpr int MESSAGE_BOX_WIDTH = Unit::WINDOW_WIDTH_PIXELS - ( ( TEXT_X - Unit::MiniBlocksToPixels( MESSAGE_BOX_PADDING ) ) * 2 );
static constexpr int MESSAGE_BOX_HEIGHT = Unit::WINDOW_HEIGHT_PIXELS - ( ( TEXT_Y - Unit::MiniBlocksToPixels( MESSAGE_BOX_PADDING ) ) * 2 );
static constexpr int LINE_LIMIT = Unit::PixelsToMiniBlocks( MESSAGE_BOX_WIDTH ) - ( MESSAGE_BOX_PADDING * 2 );

LevelMessageState::LevelMessageState( const Palette& palette, std::string message )
:
	GameState( StateID::PAUSE_STATE, palette ),
	backdrop_
	(
		( Unit::WINDOW_WIDTH_PIXELS / 2 ) - ( MESSAGE_BOX_WIDTH / 2 ),
		( Unit::WINDOW_HEIGHT_PIXELS / 2 ) - ( MESSAGE_BOX_HEIGHT / 2 ),
		MESSAGE_BOX_WIDTH,
		MESSAGE_BOX_HEIGHT
	),
	message_
	(
		Text
		(
			message,
			TEXT_X,
			TEXT_Y,
			Text::FontColor::WHITE,
			Text::FontAlign::LEFT,
			false,
			LINE_LIMIT 
		)
	)
{};

LevelMessageState::~LevelMessageState() {};

void LevelMessageState::stateUpdate()
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