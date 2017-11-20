#include "audio.hpp"
#include "main.hpp"
#include "input.hpp"
#include "message_state.hpp"
#include "render.hpp"

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
			Text::FontColor::WHITE
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
	Text::FontColor font_color,
	std::string music
)
:
	GameState( StateID::MESSAGE_STATE, palette ),
	message_ ( message, 0, 0, font_color, Text::FontAlign::CENTER, true ),
	next_state_ ( std::move( next_state ) ),
	pop_ ( pop ),
	push_ ( push )
{
	Audio::turnOffSong();
	Audio::changeSong( music );
};

MessageState::~MessageState() {};

void MessageState::stateUpdate()
{
	if ( Input::pressedMain() )
	{
		if ( pop_ )
		{
			Main::popState();
		}
		else if ( push_ )
		{
			Main::pushState( std::unique_ptr<GameState> ( std::move( next_state_ ) ) );
		}
		else
		{
			Main::changeState( std::unique_ptr<GameState> ( std::move( next_state_ ) ) );
		}
	}
};

void MessageState::stateRender()
{
	Render::colorCanvas();
	message_.render();
};