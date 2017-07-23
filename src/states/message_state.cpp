#include "main.h"
#include "input.h"
#include "message_state.h"
#include "render.h"

std::unique_ptr<MessageState> MessageState::error
(
	std::string message,
	bool pop,
	std::unique_ptr<GameState> next_state,
	bool push
)
{
	return std::unique_ptr<MessageState>
	(
		new MessageState
		(
			message,
			pop,
			std::move( next_state ),
			push,
			{ "Grayscale", 6 },
			Text::FontShade::WHITE
		)
	);
};

MessageState::MessageState
(
	std::string message,
	bool pop,
	std::unique_ptr<GameState> next_state,
	bool push,
	const Palette& palette,
	Text::FontShade font_color
)
:
	GameState( StateID::MESSAGE_STATE, palette ),
	message_ ( Text( message, 0, 0, Text::FontShade::BLACK, Text::FontAlign::CENTER, true )  ),
	next_state_ ( move( next_state ) ),
	pop_ ( pop ),
	push_ ( push ),
	font_color_ ( font_color )
{};

MessageState::~MessageState() {};

void MessageState::update()
{
	if ( Input::pressedMain() )
	{
		if ( pop_ )
		{
			Main::popState();
		}
		else if ( push_ )
		{
			Main::pushState( std::unique_ptr<GameState> ( move( next_state_ ) ) );
		}
		else
		{
			Main::changeState( std::unique_ptr<GameState> ( move( next_state_ ) ) );
		}
	}
};

void MessageState::stateRender()
{
	Render::colorCanvas();
	message_.render( nullptr, font_color_ );
};

void MessageState::init() {};
