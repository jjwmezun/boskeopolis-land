


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

    LevelMessageState::LevelMessageState( Palette::PaletteSet palette, std::string message )
    :
        message_ ( Text( message, TEXT_X, TEXT_Y, Text::FontShade::WHITE, Text::FontAlign::LEFT, false, nullptr, LINE_LIMIT ) ),
        GameState( StateID::PAUSE_STATE, palette )
    {};

    LevelMessageState::~LevelMessageState() {};

    void LevelMessageState::update( Game& game, Input& input, Graphics& graphics )
    {
        if ( input.pressed( Input::Action::CONFIRM ) || input.pressed( Input::Action::MENU ) || input.pressed( Input::Action::CANCEL ) )
        {
            game.popState();
        }
    };

    void LevelMessageState::stateRender( Graphics& graphics )
    {
        graphics.renderRect( backdrop_, 6 );
        message_.render( graphics );
    };

    void LevelMessageState::init( Game& game, Graphics& graphics )
    {
    };

    void LevelMessageState::backFromPop( Game& game, Graphics& graphics ) {};