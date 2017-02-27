

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

	constexpr sdl2::SDLRect PauseState::surface_box_;


// METHODS
//===================================

    PauseState::PauseState( Palette::PaletteSet palette, EventSystem& events, bool beaten )
    :
        GameState( StateID::PAUSE_STATE, palette ),
		option_text_ ( optionText( beaten ) ),
		option_selection_ ( PauseOption::PO_CONTINUE ),
        events_ ( events )
    {};

	std::vector<Text> PauseState::optionText( bool beaten ) const
	{
		std::vector<Text> temp;
		
		temp.push_back( Text( "Continue", surface_box_.x + 8, surface_box_.y + 16, Text::FontShade::LIGHT_MID_GRAY ) );
		
        if ( beaten )
        {
			temp.push_back( Text( "Back to Level Select", surface_box_.x + 8, surface_box_.y + 32, Text::FontShade::LIGHT_MID_GRAY ) );
        }
		else
		{
			temp.push_back( Text( "Give Up", surface_box_.x + 8, surface_box_.y + 32, Text::FontShade::LIGHT_MID_GRAY ) );
		}
		
		return temp;
	};

    PauseState::~PauseState() {};

    void PauseState::update( Game& game, const Input& input, Graphics& graphics )
    {
        if ( input.pressed( Input::Action::MOVE_UP ) || input.pressed( Input::Action::MOVE_DOWN ) )
        {
			switch ( option_selection_ )
			{
				case ( PauseOption::PO_CONTINUE ):
                	option_selection_ = PauseOption::PO_QUIT;
				break;
				case ( PauseOption::PO_QUIT ):
                	option_selection_ = PauseOption::PO_CONTINUE;
				break;
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

        for ( int i = 0; i < NUM_O_PAUSE_OPTIONS; ++i )
        {
            Text::FontShade text_color = Text::FontShade::__NULL;

            if ( ( int )option_selection_ == i )
			{
                text_color = Text::FontShade::WHITE;
			}
			
            option_text_.at( i ).render( graphics, nullptr, text_color );
        }
    };

    void PauseState::init( Game& game, Graphics& graphics )
    {
    };

    void PauseState::backFromPop( Game& game, Graphics& graphics ) {};
