#include "frame.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "new_game_confirm_prompt_state.hpp"
#include "overworld_state.hpp"
#include "render.hpp"
#include "save.hpp"
#include "unit.hpp"
#include "wtext_obj.hpp"

static constexpr int WIDTH_CHARACTERS = WTextCharacter::SIZE_PIXELS * 16;
static constexpr int FRAME_PADDING = 9;
static constexpr int FRAME_WIDTH = WIDTH_CHARACTERS + ( FRAME_PADDING * 2 );
static constexpr int FRAME_X = ( int )( ( double )( Unit::WINDOW_WIDTH_PIXELS - FRAME_WIDTH ) / 2.0 );

NewGameConfirmPromptState::NewGameConfirmPromptState( std::u32string save_name )
:
    GameState( StateID::NEW_GAME_STATE ),
    save_name_ ( save_name ),
    confirm_ ( true ),
    error_ ( false )
{};

NewGameConfirmPromptState::~NewGameConfirmPromptState() {};

void NewGameConfirmPromptState::stateUpdate()
{
    if ( error_ )
    {
        if ( Input::pressed( Input::Action::CONFIRM ) )
        {
            Main::popState();
        }
    }
    else
    {
        if ( Input::pressed( Input::Action::CONFIRM ) )
        {
            if ( confirm_ )
            {
                Save save = Save::createNew( save_name_ );
                Save::Status save_status = save.save();
                if ( save_status == Save::Status::FAILED_TO_LOAD_MAIN_SAVE )
                {
                    error_ = true;
                }
                else
                {
                    Inventory::load( save );
                    Main::changeState( std::unique_ptr<OverworldState> ( new OverworldState( 0, true ) ) );
                }
            }
            else
            {
                Main::popState();
            }
        }
        if ( Input::pressed( Input::Action::MOVE_UP ) || Input::pressed( Input::Action::MOVE_DOWN ) )
        {
            confirm_ = !confirm_;
        }
    }
};

void NewGameConfirmPromptState::stateRender()
{
    std::u32string string = ( error_ )
        ? mezun::charToChar32String( "Failed to create save file somehow. Something weird’s going on with your operating system." )
        : mezun::stringReplace( mezun::charToChar32String( "¿Name new save “%fn”?\n\n Confirm\n Cancel" ), U"%fn", save_name_ );
    WTextObj text { string, FRAME_X, 0, WTextCharacter::Color::BLACK, FRAME_WIDTH, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, FRAME_PADDING, 8, WTextObj::VAlign::CENTER, Unit::WINDOW_HEIGHT_PIXELS };
    const int frame_height = ( text.getNumberOfLines() * WTextCharacter::SIZE_PIXELS ) + ( FRAME_PADDING * 2 );
    const int y = ( int )( ( double )( Unit::WINDOW_HEIGHT_PIXELS - frame_height ) / 2.0 );
    Frame frame { FRAME_X, y, FRAME_WIDTH, frame_height };
    frame.render();
    text.render();

    if ( !error_ )
    {
        const int selection_x = FRAME_X + 8;
        const int selection_y = y + frame_height - ( ( confirm_ ) ? 24 : 16 );
        Render::renderObject( "bg/arrow-icon.png", { 0, 0, 8, 8 }, { selection_x, selection_y, 8, 8 } );
    }
};

void NewGameConfirmPromptState::init()
{
};