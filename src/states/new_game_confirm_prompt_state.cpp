#include "frame.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "new_game_confirm_prompt_state.hpp"
#include "render.hpp"
#include "save.hpp"
#include "unit.hpp"
#include "wtext_obj.hpp"

NewGameConfirmPromptState::NewGameConfirmPromptState( Save& save, std::u32string question, bool& confirm )
:
    PromptState( mezun::stringReplace( mezun::merge32Strings( question, mezun::charToChar32String( "\n\n Confirm\n Cancel" ) ), U"%fn", save.name() ), 1 ),
    save_ ( save ),
    error_ ( false ),
    confirm_ ( confirm )
{};

NewGameConfirmPromptState::~NewGameConfirmPromptState() {};

void NewGameConfirmPromptState::stateUpdate()
{
    if ( error_ )
    {
        if ( Input::pressed( Input::Action::CONFIRM ) )
        {
            confirm_ = false;
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
                    confirm_ = true;
                    Main::popState();
                }
            }
            else
            {
                confirm_ = false;
                Main::popState();
            }
        }
        if ( Input::pressed( Input::Action::MOVE_UP ) || Input::pressed( Input::Action::MOVE_DOWN ) )
        {
            selection_ = ( selection_ == 1 ) ? 0 : 1;
        }
    }
};