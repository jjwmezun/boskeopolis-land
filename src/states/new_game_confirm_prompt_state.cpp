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

NewGameConfirmPromptState::NewGameConfirmPromptState( Save& save, std::u32string question )
:
    PromptState
    (
        mezun::merge32Strings
        (
            mezun::stringReplace
            (
                question,
                U"%fn",
                save.name()
            ),
            mezun::charToChar32String( "\n\n Confirm\n Cancel" )
        ),
        1
    ),
    save_ ( save ),
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
            if ( selection_ == 1 )
            {
                Save::Status save_status = save_.save();
                if ( save_status == Save::Status::FAILED_TO_LOAD_MAIN_SAVE )
                {
                    error_ = true;
                    text_ = mezun::charToChar32String( "Failed to create save file somehow. Something weird’s going on with your operating system." );
                }
                else
                {
                    Main::popState();
                    //Inventory::load( save_ );
                    //Main::changeState( std::unique_ptr<OverworldState> ( new OverworldState( 0, true ) ) );
                }
            }
            else
            {
                save_.setAsDeleted();
                Main::popState();
            }
        }
        if ( Input::pressed( Input::Action::MOVE_UP ) || Input::pressed( Input::Action::MOVE_DOWN ) )
        {
            selection_ = ( selection_ == 1 ) ? 0 : 1;
        }
    }
};