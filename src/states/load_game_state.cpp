#include <filesystem>
#include "load_game_state.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "render.hpp"
#include "wmessage_state.hpp"

LoadGameState::LoadGameState()
:
    GameState( StateID::LOAD_GAME_STATE )
{};

LoadGameState::~LoadGameState() {};

void LoadGameState::stateUpdate()
{

};

void LoadGameState::stateRender()
{
    Render::colorCanvas( 4 );
    for ( int i = 0; i < saves_.size(); ++i )
    {
        const int y = 24 + ( 32 * i );
        Render::renderObject( "bg/load-game-frame.png", { 0, 0, 384, 32 }, { 8, y, 384, 32 } );
    }
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