#include "frame.hpp"
#include "input.hpp"
#include "main.hpp"
#include "prompt_state.hpp"
#include "render.hpp"
#include "unit.hpp"
#include "wtext_obj.hpp"

static constexpr int WIDTH_CHARACTERS = WTextCharacter::SIZE_PIXELS * 16;
static constexpr int FRAME_PADDING = 9;
static constexpr int FRAME_WIDTH = WIDTH_CHARACTERS + ( FRAME_PADDING * 2 );
static constexpr int FRAME_X = ( int )( ( double )( Unit::WINDOW_WIDTH_PIXELS - FRAME_WIDTH ) / 2.0 );

PromptState::PromptState( std::u32string text, int selection )
:
    GameState( StateID::NEW_GAME_STATE, { "Cement Gray", 1 } ),
    selection_ ( selection ),
    text_ ( text )
{};

PromptState::~PromptState() {};

void PromptState::stateUpdate()
{
    if ( Input::pressed( Input::Action::CONFIRM ) || Input::pressed( Input::Action::CANCEL ) )
    {
        Main::popState();
    }
};

void PromptState::stateRender()
{
    WTextObj text { text_, FRAME_X, 0, WTextCharacter::Color::BLACK, FRAME_WIDTH, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, FRAME_PADDING, 8, WTextObj::VAlign::CENTER, Unit::WINDOW_HEIGHT_PIXELS };
    const int frame_height = ( text.getNumberOfLines() * WTextCharacter::SIZE_PIXELS ) + ( FRAME_PADDING * 2 );
    const int y = ( int )( ( double )( Unit::WINDOW_HEIGHT_PIXELS - frame_height ) / 2.0 );
    Frame frame { FRAME_X, y, FRAME_WIDTH, frame_height };
    frame.render();
    text.render();

    if ( selection_ > -1 )
    {
        const int selection_x = FRAME_X + 8;
        const int selection_y = y + frame_height - ( 16 + ( selection_ * 8 ) );
        Render::renderObject( "bg/arrow-icon.png", { 0, 0, 8, 8 }, { selection_x, selection_y, 8, 8 } );
    }
};

void PromptState::init()
{
};

void PromptState::extraRender() {};