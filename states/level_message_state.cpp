


// Name
//===================================
//
// LevelMessageState
//


// DEPENDENCIES
//===================================

    #include "game.h"
    #include "input.h"
    #include "level_message_state.h"


// STATIC PROPERTIES
//===================================

    constexpr sdl2::SDLRect LevelMessageState::backdrop_;


// METHODS
//===================================

    LevelMessageState::LevelMessageState( const Palette& palette, std::string message )
    :
        GameState( StateID::PAUSE_STATE, palette ),
        message_
		(
			Text
			(
				message,
				TEXT_X,
				TEXT_Y,
				Text::FontShade::WHITE,
				Text::FontAlign::LEFT,
				false,
				LINE_LIMIT 
			)
		)
    {};

    LevelMessageState::~LevelMessageState() {};

    void LevelMessageState::update( Game& game, const Input& input, Graphics& graphics )
    {
        if ( input.pressedMain() )
        {
            game.popState();
        }
    };

    void LevelMessageState::stateRender( Graphics& graphics )
    {
        graphics.renderRect( backdrop_, 6 );
        message_.render( graphics );
    };

    void LevelMessageState::init( Game& game, Graphics& graphics ) {};

    void LevelMessageState::backFromPop( Game& game, Graphics& graphics ) {};
