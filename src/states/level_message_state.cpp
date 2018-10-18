#include "audio.hpp"
#include "main.hpp"
#include "input.hpp"
#include "inventory_level.hpp"
#include "level_message_state.hpp"
#include "render.hpp"
#include "text_component_gradual.hpp"
#include <memory>

static constexpr int MESSAGE_BOX_HEIGHT_MINIBLOCKS = 12;
static constexpr int MESSAGE_BOX_WIDTH_PIXELS = Unit::MiniBlocksToPixels( LevelMessageState::MESSAGE_BOX_WIDTH_MINIBLOCKS );
static constexpr int MESSAGE_BOX_HEIGHT_PIXELS = Unit::MiniBlocksToPixels( MESSAGE_BOX_HEIGHT_MINIBLOCKS );
static constexpr int MESSAGE_BOX_PADDING_PIXELS = Unit::MiniBlocksToPixels( LevelMessageState::MESSAGE_BOX_PADDING_MINIBLOCKS );
static constexpr int MESSAGE_BOX_X = ( Unit::WINDOW_WIDTH_PIXELS / 2 ) - ( MESSAGE_BOX_WIDTH_PIXELS / 2 );
static constexpr int MESSAGE_BOX_Y = ( ( Unit::WINDOW_HEIGHT_PIXELS - InventoryLevel::HEIGHT ) / 2 ) - ( MESSAGE_BOX_HEIGHT_PIXELS / 2 );
static constexpr int TEXT_X = MESSAGE_BOX_X + MESSAGE_BOX_PADDING_PIXELS;
static constexpr int TEXT_Y = MESSAGE_BOX_Y + MESSAGE_BOX_PADDING_PIXELS;
static constexpr int BORDER_WIDTH = 8;

LevelMessageState::LevelMessageState( const Palette& palette, std::string message )
:
	GameState( StateID::PAUSE_STATE, palette ),
	backdrop_
	(
		MESSAGE_BOX_X,
		MESSAGE_BOX_Y,
		MESSAGE_BOX_WIDTH_PIXELS,
		MESSAGE_BOX_HEIGHT_PIXELS
	),
	border_
	(
		MESSAGE_BOX_X - BORDER_WIDTH,
		MESSAGE_BOX_Y - BORDER_WIDTH,
		MESSAGE_BOX_WIDTH_PIXELS + ( BORDER_WIDTH * 2 ),
		MESSAGE_BOX_HEIGHT_PIXELS + ( BORDER_WIDTH * 2 )
	),
	message_
	(
		message,
		TEXT_X,
		TEXT_Y,
		Text::FontColor::WHITE,
		Text::FontAlign::LEFT,
		Text::FontColor::__NULL,
		false,
		LINE_LIMIT,
		0,
		1,
		std::make_unique<TextComponentGradual> ( 2 )
	)
{
	Audio::playSound( Audio::SoundType::PAUSE );
};

LevelMessageState::~LevelMessageState() {};

void LevelMessageState::stateUpdate()
{
	if ( Input::pressedMain() )
	{
		Main::popState();
	}

	message_.update();
};

void LevelMessageState::stateRender()
{
	Render::renderRect( border_, 1 );
	Render::renderRect( backdrop_, 6 );
	message_.render();
};
