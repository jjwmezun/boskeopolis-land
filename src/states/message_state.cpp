#include "audio.hpp"
#include "main.hpp"
#include "input.hpp"
#include "message_state.hpp"
#include "render.hpp"
#include "text_component_flashing.hpp"

MessageState::MessageState
(
	std::string message,
	Type type,
	Palette palette,
	std::unique_ptr<GameState> next_state,
	Text::FontColor text_color,
	Text::FontColor shadow_color,
	std::string music,
	bool loop_music,
	bool flash
)
:
	GameState( StateID::MESSAGE_STATE, palette ),
	message_ ( message, 0, 0, text_color, Text::FontAlign::CENTER, shadow_color, true, Text::DEFAULT_LINE_LENGTH, -1, 1, ( flash ) ? std::make_unique<TextComponentFlashing> ( 4 ) : nullptr ),
	next_state_ ( std::move( next_state ) ),
	type_ ( type )
{
	Audio::turnOffSong();
	Audio::changeSong( music, loop_music );
};

MessageState::~MessageState() {};

void MessageState::stateUpdate()
{
	message_.update();

	if ( Input::pressedMain() )
	{
		switch ( type_ )
		{
			case ( Type::CHANGE ):
				Main::changeState( std::unique_ptr<GameState> ( std::move( next_state_ ) ) );
			break;

			case ( Type::POP ):
				Main::popState( true );
			break;

			case ( Type::PUSH ):
				Main::pushState( std::unique_ptr<GameState> ( std::move( next_state_ ) ), true );
			break;
		}
	}
};

void MessageState::stateRender()
{
	Render::colorCanvas();
	message_.render();
};