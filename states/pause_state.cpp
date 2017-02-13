

// Name
//===================================
//
// PauseState
//


// DEPENDENCIES
//===================================

    #include "game.h"
    #include "input.h"
    #include "pause_state.h"
    #include "level_select_state.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    PauseState::PauseState( Palette::PaletteSet palette, EventSystem& events, bool beaten )
    :
        GameState( StateID::PAUSE_STATE, palette ),
        events_ ( events )
    {
        if ( !beaten )
        {
            option_text_[ (int)PauseOption::PO_QUIT ].words_ = "Give Up";
        }
    };

    PauseState::~PauseState() {};

    void PauseState::update( Game& game, Input& input, Graphics& graphics )
    {
        if ( input.pressed( Input::Action::MOVE_UP ) || input.pressed( Input::Action::MOVE_DOWN ) )
        {
            if ( option_selection_ == PauseOption::PO_CONTINUE )
            {
                option_selection_ = PauseOption::PO_QUIT;
            }
            else
            {
                option_selection_ = PauseOption::PO_CONTINUE;
            }
        }

        if ( input.pressed( Input::Action::CONFIRM ) )
        {
            if ( option_selection_ == PauseOption::PO_QUIT )
            {
                events_.quit_level_ = true;
            }
            game.popState();
        }

        if ( input.pressed( Input::Action::MENU ) )
        {
            game.popState();
        }
    };

    void PauseState::stateRender( Graphics& graphics )
    {
        graphics.renderRect( surface_box_, 6 );

        for ( int i = 0; i < (int)PauseOption::PO_LIMIT; ++i )
        {
            Text::FontShade text_color = Text::FontShade::__NULL;

            if ( (int)option_selection_ == i )
                text_color = Text::FontShade::WHITE;

            option_text_[ i ].render( graphics, nullptr, text_color );
        }
    };

    void PauseState::init( Game& game, Graphics& graphics )
    {
    };

    void PauseState::backFromPop( Game& game, Graphics& graphics ) {};
