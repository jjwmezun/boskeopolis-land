#include "audio.hpp"
#include "main.hpp"
#include "input.hpp"
#include "inventory_level.hpp"
#include "level_message_state.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "render.hpp"
#include "text_component_gradual.hpp"
#include <memory>

static constexpr int WIDTH = 368;
static constexpr int HEIGHT = 128;
static constexpr int X = ( int )( ( double )( Unit::WINDOW_WIDTH_PIXELS - WIDTH ) / 2.0 );
static constexpr int Y = ( int )( ( double )( Unit::WINDOW_HEIGHT_PIXELS - InventoryLevel::HEIGHT - HEIGHT ) / 2.0 );
static constexpr int PADDING = 16;

static constexpr int MESSAGE_BOX_HEIGHT_MINIBLOCKS = 12;
static constexpr int MESSAGE_BOX_WIDTH_PIXELS = Unit::MiniBlocksToPixels( LevelMessageState::MESSAGE_BOX_WIDTH_MINIBLOCKS );
static constexpr int MESSAGE_BOX_HEIGHT_PIXELS = Unit::MiniBlocksToPixels( MESSAGE_BOX_HEIGHT_MINIBLOCKS );
static constexpr int MESSAGE_BOX_PADDING_PIXELS = Unit::MiniBlocksToPixels( LevelMessageState::MESSAGE_BOX_PADDING_MINIBLOCKS );
static constexpr int MESSAGE_BOX_X = ( Unit::WINDOW_WIDTH_PIXELS / 2 ) - ( MESSAGE_BOX_WIDTH_PIXELS / 2 );
static constexpr int MESSAGE_BOX_Y = ( ( Unit::WINDOW_HEIGHT_PIXELS - InventoryLevel::HEIGHT ) / 2 ) - ( MESSAGE_BOX_HEIGHT_PIXELS / 2 );
static constexpr int TEXT_X = MESSAGE_BOX_X + MESSAGE_BOX_PADDING_PIXELS;
static constexpr int TEXT_Y = MESSAGE_BOX_Y + MESSAGE_BOX_PADDING_PIXELS;
static constexpr int BORDER_WIDTH = 8;

LevelMessageState::LevelMessageState( const Palette& palette, std::u32string message )
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
	({
		message,
		X,
		Y,
		WTextCharacter::Color::BLACK,
		MESSAGE_BOX_WIDTH_PIXELS,
		WTextObj::Align::LEFT,
		WTextCharacter::Color::__NULL,
		PADDING,
		PADDING
	}),
	frame_ { "bg/level-message-frame.png", { 0, 0, WIDTH, HEIGHT }, { X, Y, WIDTH, HEIGHT } }
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
	frame_.render();
	message_.render();
};
