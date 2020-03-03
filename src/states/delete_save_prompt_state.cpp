#include "delete_save_prompt_state.hpp"
#include "input.hpp"
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "save.hpp"

DeleteSavePromptState::DeleteSavePromptState( Save& save )
:
    PromptState( mezun::stringReplace( mezun::charToChar32String( "¿Are you sure you want to delete save file “%n?” This can’t be undone.\n No\n Yes" ), U"%n", save.name() ), 1 ),
    save_ ( save )
{};

DeleteSavePromptState::~DeleteSavePromptState() {};

void DeleteSavePromptState::stateUpdate()
{
    if ( selection_ == -1 )
    {
        if ( Input::pressed( Input::Action::CONFIRM ) || Input::pressed( Input::Action::CANCEL ) )
        {
            Main::popState();
        }
    }
    else if ( Input::pressed( Input::Action::CONFIRM ) )
    {
        if ( selection_ == 0 )
        {
            Save::Status status = save_.deleteSave();
            selection_ = -1;
            text_ = mezun::stringReplace
            (
                ( ( Save::testAnyDeleteFailStatus( status ) ) ? mezun::charToChar32String( "Error trying to remove save “%n”. File was already deleted." ) : mezun::charToChar32String( "Save file “%n” deleted." ) ),
                U"%n",
                save_.name()
            );
        }
        else
        {
            Main::popState();
        }
    }
    else if ( Input::pressed( Input::Action::CANCEL ) )
    {
        Main::popState();
    }
    else if ( Input::pressed( Input::Action::MOVE_DOWN ) || Input::pressed( Input::Action::MOVE_UP ) )
    {
        selection_ = ( selection_ == 0 ) ? 1 : 0;
    }
};