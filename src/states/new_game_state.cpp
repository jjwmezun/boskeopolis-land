#include "mezun_helpers.hpp"
#include "new_game_state.hpp"
#include "wtext_obj.hpp"
#include "render.hpp"
#include "unit.hpp"

#include <SDL2/SDL.h>
#include <iostream>
#include "input.hpp"

NewGameState::NewGameState()
:
    GameState( StateID::NEW_GAME_STATE )
{};

NewGameState::~NewGameState() {};

void NewGameState::stateUpdate()
{
    const bool* letters = Input::getLetters();
    for ( int i = 0; i < Input::NUMBER_OF_LETTERS; ++i )
    {
        if ( letters[ i ] )
        {
            name_ += Input::getLetterCharacter( i );
        }
    }
};

void NewGameState::stateRender()
{
    Render::colorCanvas( 4 );
    WTextObj text { name_, 0, 0, WTextCharacter::Color::WHITE, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::CENTER, WTextCharacter::Color::BLACK, 8, 8, WTextObj::VAlign::CENTER, Unit::WINDOW_HEIGHT_PIXELS };
    text.render();
};

void NewGameState::init()
{
};