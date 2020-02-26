#include "frame.hpp"
#include "mezun_helpers.hpp"
#include "new_game_confirm_prompt_state.hpp"
#include "wtext_obj.hpp"
#include "render.hpp"
#include "unit.hpp"
#include "input.hpp"

static constexpr int WIDTH_CHARACTERS = WTextCharacter::SIZE_PIXELS * 16;
static constexpr int FRAME_PADDING = 9;
static constexpr int FRAME_WIDTH = WIDTH_CHARACTERS + ( FRAME_PADDING * 2 );
static constexpr int FRAME_X = ( int )( ( double )( Unit::WINDOW_WIDTH_PIXELS - FRAME_WIDTH ) / 2.0 );

NewGameConfirmPromptState::NewGameConfirmPromptState( std::u32string save_name )
:
    GameState( StateID::NEW_GAME_STATE ),
    save_name_ ( save_name )
{};

NewGameConfirmPromptState::~NewGameConfirmPromptState() {};

void NewGameConfirmPromptState::stateUpdate()
{
};

void NewGameConfirmPromptState::stateRender()
{
    std::u32string string { mezun::stringReplace( mezun::charToChar32String( "¿Name new save “%fn”?\n\n Confirm\n Cancel" ), U"%fn", save_name_ ) };
    WTextObj text { string, FRAME_X, 0, WTextCharacter::Color::BLACK, FRAME_WIDTH, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, FRAME_PADDING, 8, WTextObj::VAlign::CENTER, Unit::WINDOW_HEIGHT_PIXELS };
    const int frame_height = ( text.getNumberOfLines() * WTextCharacter::SIZE_PIXELS ) + ( FRAME_PADDING * 2 );
    const int y = ( int )( ( double )( Unit::WINDOW_HEIGHT_PIXELS - frame_height ) / 2.0 );
    Frame frame { FRAME_X, y, FRAME_WIDTH, frame_height };
    frame.render();
    text.render();
};

void NewGameConfirmPromptState::init()
{
};