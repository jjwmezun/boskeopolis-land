


// Name
//===================================
//
// TitleState
//


// DEPENDENCIES
//===================================

    #include "game.h"
    #include "input.h"
    #include <fstream>
    #include "level_select_state.h"
    #include "title_state.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    TitleState::TitleState() : GameState( StateID::TITLE_STATE ) {};

    TitleState::~TitleState() {};

    void TitleState::update( Game& game, Input& input, Graphics& graphics )
    {
        if ( input.pressed( Input::Action::CONFIRM ) )
        {
            switch( (Option)selection_.value() )
            {
                case ( Option::NEW ):
                    game.changeState( std::unique_ptr<GameState> ( LevelSelectState::newLevelSelect() ) );
                break;

                case ( Option::LOAD ):
                    if ( can_load_ )
                        game.changeState( std::unique_ptr<GameState> ( LevelSelectState::loadLevelSelect() ) );
                break;

                case ( Option::QUIT ):
                    game.quit();
                break;
            }
        }

        if ( input.pressed( Input::Action::MOVE_UP ) )
        {
            --selection_;
        }
        else if ( input.pressed( Input::Action::MOVE_DOWN ) )
        {
            ++selection_;
        }

        highlight_rect_.y = OPTIONS_TOP_Y + Unit::MiniBlocksToPixels( selection_.value() );
    };

    void TitleState::stateRender( Graphics& graphics )
    {
        graphics.renderRect( highlight_rect_, 6 );

        for ( int i = 0; i < option_text_.size(); ++i )
        {
            Text::FontShade shade = Text::FontShade::BLACK;

            if ( i == (int)Option::OPTIONS || ( !can_load_ && i == (int)Option::LOAD ) )
            {
                shade = Text::FontShade::LIGHT_MID_GRAY;
            }
            else if ( i == selection_.value() )
            {
                shade = Text::FontShade::WHITE;
            }

            Text::renderText( graphics, option_text_.at( i ), 0, OPTIONS_TOP_Y + Unit::MiniBlocksToPixels( i ), nullptr, shade, -1, Text::FontAlign::CENTER );
        }

        logo_gfx_.render( graphics, logo_rect_, nullptr );
    };

    void TitleState::init( Game& game, Graphics& graphics )
    {
        std::ifstream ifs( Game::savePath() );

        if ( ifs.is_open() )
        {
            can_load_ = true;
        }
        else
        {
            can_load_ = false;
        }

        ifs.close();
    };

    void TitleState::backFromPop( Game& game, Graphics& graphics ) {};
