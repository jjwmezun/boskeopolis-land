

// Name
//===================================
//
// MessageState
//


// DEPENDENCIES
//===================================

    #include "game.h"
    #include "input.h"
    #include "message_state.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    MessageState::MessageState
    (
        std::string message,
        bool pop,
        std::unique_ptr<GameState> next_state,
        bool push,
        Palette::PaletteSet palette,
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

    void MessageState::update( Game& game, Input& input, Graphics& graphics )
    {
        if ( input.pressedMain() )
        {
            if ( pop_ )
            {
                game.popState();
            }
            else if ( push_ )
			{
                game.pushState( std::unique_ptr<GameState> ( move( next_state_ ) ) );
			}
            else
			{
                game.changeState( std::unique_ptr<GameState> ( move( next_state_ ) ) );
			}
        }
    };

    void MessageState::stateRender( Graphics& graphics )
    {
        graphics.colorCanvas();
        message_.render( graphics, nullptr, font_color_ );
    };

    void MessageState::init( Game& game, Graphics& graphics ) {};

    void MessageState::backFromPop( Game& game, Graphics& graphics ) {};
