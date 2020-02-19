#include "audio.hpp"
#include "main.hpp"
#include "input.hpp"
#include "wmessage_state.hpp"
#include "render.hpp"
#include "text_component_flashing.hpp"

std::unique_ptr<WMessageState> WMessageState::generateErrorMessage( std::u32string message, Type type, std::unique_ptr<GameState> next_state )
{
	return std::unique_ptr<WMessageState>
	(
		new WMessageState
		(
			WTextObj::MessageData{ message, WTextCharacter::Color::WHITE, WTextCharacter::Color::__NULL },
			type,
			{ "Grayscale", 6 },
			std::move( next_state )
		)
	);
};

WMessageState::WMessageState
(
	WTextObj::MessageData message,
	Type type,
	Palette palette,
	std::unique_ptr<GameState> next_state,
	std::string music,
	bool loop_music,
	bool flash
)
:
	GameState( StateID::MESSAGE_STATE, palette ),
	text_ ( message.text, 0, 0, message.color, WTextObj::DEFAULT_WIDTH, WTextObj::Align::CENTER, message.shadow, 16, 16, WTextObj::VAlign::CENTER ),
    text_gfx_ (),
	next_state_ ( std::move( next_state ) ),
	type_ ( type )
{
	Audio::turnOffSong();
	Audio::changeSong( music, loop_music );
};

WMessageState::~WMessageState()
{
    text_gfx_.destroy();
};

void WMessageState::init()
{
    text_.generateTexture( text_gfx_ );
};

void WMessageState::stateUpdate()
{
	if ( Input::pressedMain() )
	{
		switch ( type_ )
		{
			case ( Type::CHANGE ):
            {
				Main::changeState( std::unique_ptr<GameState> ( std::move( next_state_ ) ) );
            }
			break;

			case ( Type::POP ):
            {
				Main::popState( true );
            }
			break;

			case ( Type::PUSH ):
            {
				Main::pushState( std::unique_ptr<GameState> ( std::move( next_state_ ) ), true );
            }
			break;
		}
	}
};

void WMessageState::stateRender()
{
	Render::colorCanvas();
	text_.render();
};
