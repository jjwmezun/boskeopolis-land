#include <filesystem>
#include "load_game_state.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "overworld_state.hpp"
#include "render.hpp"
#include "title_state.hpp"
#include "wmessage_state.hpp"

static constexpr int NAME_LIMIT = 45;

LoadGameState::LoadGameState()
:
    GameState( StateID::LOAD_GAME_STATE ),
    selection_ ( 0 ),
    state_ ( State::SELECT ),
    name_ (),
    timer_ ( 0 )
{};

LoadGameState::~LoadGameState() {};

void LoadGameState::stateUpdate()
{
    switch ( state_ )
    {
        case ( State::SELECT ):
        {
            if ( Input::pressed( Input::Action::CANCEL ) || Input::pressed( Input::Action::MENU ) )
            {
                Main::changeState( std::unique_ptr<TitleState> ( new TitleState() ) );
            }
            else if ( Input::pressed( Input::Action::CONFIRM ) )
            {
                if ( selection_ == maxSelection() )
                {
                    state_ = State::NAMING;
                }
                else
                {
                    Main::changeState( std::unique_ptr<OverworldState> ( new OverworldState( 0 ) ) );
                }
            }
            else if ( Input::pressed( Input::Action::MOVE_DOWN ) )
            {
                ++selection_;
                if ( selection_ > maxSelection() )
                {
                    selection_ = 0;
                }
            }
            else if ( Input::pressed( Input::Action::MOVE_DOWN ) )
            {
                --selection_;
                if ( selection_ < 0 )
                {
                    selection_ = maxSelection();
                }
            }
        }
        break;
        case ( State::NAMING ):
        {
            if ( nameLessThanLimit() )
            {
                const bool* letters = Input::getLetters();
                for ( int i = 0; i < Input::NUMBER_OF_LETTERS; ++i )
                {
                    if ( letters[ i ] )
                    {
                        name_ += Input::getLetterCharacter( i );
                    }
                }
            }

            if ( Input::pressedBackspace() && !name_.empty() )
            {
                name_.pop_back();
            }
        }
        break;
    }
};

void LoadGameState::stateRender()
{
    Render::colorCanvas( 4 );
    int i = 0;
    while ( i < saves_.size() )
    {
        const int y = 24 + ( 32 * i );
        const std::string texture = ( selection_ == i ) ? "bg/load-game-frame.png" : "bg/load-game-frame-unselected.png";
        Render::renderObject( texture, { 0, 0, 384, 32 }, { 8, y, 384, 32 } );
        ++i;
    }
    const int y = 24 + ( 32 * i );
    const std::string texture = ( selection_ == i ) ? "bg/load-game-frame.png" : "bg/load-game-frame-unselected.png";
    Render::renderObject( texture, { 0, 0, 384, 32 }, { 8, y, 384, 32 } );

    WTextObj text;
    switch ( state_ )
    {
        case ( State::SELECT ):
        {
            text = { U"New Game", 0, y, WTextCharacter::Color::BLACK, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 16, 8 };
        }
        break;
        case ( State::NAMING ):
        {
            const std::u32string end = ( timer_ > 7 && nameLessThanLimit() ) ? U"_" : U"";
            text = { mezun::merge32Strings( name_, end ), 0, y, WTextCharacter::Color::BLACK, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 16, 8 };
            ++timer_;
            if ( timer_ > 15 )
            {
                timer_ = 0;
            }
        }
        break;
    }
    text.render();
};

void LoadGameState::init()
{
    for ( auto& file : std::filesystem::directory_iterator( Main::saveDirectory() ) )
    {
        Save save = Save::loadFromFile( file.path() );
        if ( save.hasError() )
        {
            Main::pushState
            (
                WMessageState::generateErrorMessage
                (
                    Localization::getCurrentLanguage().getSaveCorruptionErrorMessage( file.path() ),
                    WMessageState::Type::POP,
                    nullptr
                )
            );
        }
        else
        {
            saves_.push_back( save );
        }
    }
};

int LoadGameState::maxSelection() const
{
    return saves_.size();
};

bool LoadGameState::nameLessThanLimit() const
{
    return name_.size() < NAME_LIMIT;
};