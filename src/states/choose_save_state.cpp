#include "audio.hpp"
#include <filesystem>
#include "frame.hpp"
#include "choose_save_state.hpp"
#include "counter_t.hpp"
#include "delete_save_prompt_state.hpp"
#include "inventory.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "new_game_confirm_prompt_state.hpp"
#include "overworld_state.hpp"
#include "render.hpp"
#include "title_state.hpp"
#include "wmessage_state.hpp"

static constexpr int NAME_LIMIT = 45;
static constexpr int getBoxSizeFromStringSize( int string_size )
{
    return string_size * WTextCharacter::SIZE_PIXELS + 24;
};

ChooseSaveState::ChooseSaveState()
:
    GameState( StateID::LOAD_GAME_STATE, { "Cement Gray", 1 } ),
    saves_ (),
    selection_ ( 0 ),
    state_ ( State::SELECT ),
    name_ (),
    timer_ ( 0 ),
    title_ ( { U"Choose a Save", 0, 8, WTextCharacter::Color::LIGHT_GRAY, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::CENTER, WTextCharacter::Color::BLACK, 8 } ),
    bg_ (),
    bottom_selection_ ( 0 ),
    temp_save_ ( Save::createEmpty() ),
    new_save_confirm_ ( false )
{};

ChooseSaveState::~ChooseSaveState() {};

void ChooseSaveState::stateUpdate()
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
                Audio::playSound( Audio::SoundType::CONFIRM );
                if ( selection_ == maxSelection() )
                {
                    state_ = State::NAMING;
                    bottom_selection_ = 0;
                    name_ = U"";
                }
                else
                {
                    bottom_selection_ = 0;
                    state_ = State::LOADING;
                }
            }
            else if ( Input::pressed( Input::Action::MOVE_DOWN ) )
            {
                Audio::playSound( Audio::SoundType::SELECT );
                ++selection_;
                if ( selection_ > maxSelection() )
                {
                    selection_ = 0;
                }
            }
            else if ( Input::pressed( Input::Action::MOVE_UP ) )
            {
                Audio::playSound( Audio::SoundType::SELECT );
                --selection_;
                if ( selection_ < 0 )
                {
                    selection_ = maxSelection();
                }
            }
        }
        break;
        case ( State::NAMING ):
        case ( State::COPY ):
        {
            if ( Input::pressed( Input::Action::MOVE_DOWN ) || Input::pressed( Input::Action::MOVE_UP ) )
            {
                Audio::playSound( Audio::SoundType::SELECT );
                bottom_selection_ = ( bottom_selection_ == 0 ) ? 1 : 0;
            }
            else if ( bottom_selection_ > 0 && ( Input::pressed( Input::Action::MOVE_LEFT ) || Input::pressed( Input::Action::MOVE_RIGHT ) ) )
            {
                Audio::playSound( Audio::SoundType::SELECT );
                bottom_selection_ = ( bottom_selection_ == 1 ) ? 2 : 1;
            }

            switch ( bottom_selection_ )
            {
                case ( 0 ):
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
                    ++timer_;
                    if ( timer_ > 15 )
                    {
                        timer_ = 0;
                    }
                }
                break;

                case ( 1 ):
                {
                    if ( Input::pressed( Input::Action::CONFIRM ) )
                    {
                        if ( name_.empty() )
                        {
                            Audio::playSound( Audio::SoundType::CANCEL );
                        }
                        else
                        {
                            if ( testNameAlreadyInUse( name_ ) )
                            {
                                Main::pushState( std::unique_ptr<PromptState> ( new PromptState( mezun::stringReplace( mezun::charToChar32String( "Filename “%n” already in use. Please pick a different name." ), U"%n", name_ ) ) ) );
                                Audio::playSound( Audio::SoundType::CANCEL );
                            }
                            else
                            {
                                temp_save_ = ( state_ == State::NAMING ) ? Save::createNew( name_ ) : saves_[ selection_ ].copy( name_ );
                                std::u32string question = ( state_ == State::NAMING ) ? mezun::charToChar32String( "¿Name new save “%fn”?" ) : mezun::stringReplace( mezun::charToChar32String( "¿Name copy o’ “%ofn” as “%fn”?" ), U"%ofn", saves_[ selection_ ].name() );
                                Main::pushState( std::unique_ptr<NewGameConfirmPromptState> ( new NewGameConfirmPromptState( temp_save_, question, new_save_confirm_ ) ) );
                                Audio::playSound( Audio::SoundType::CONFIRM );
                                state_ = ( state_ == State::NAMING ) ? State::CREATING_NEW_SAVE : State::CREATING_COPY_SAVE;
                            }
                        }
                    }
                    else if ( Input::pressed( Input::Action::CANCEL ) )
                    {
                        exitNaming();
                    }
                }
                break;

                case ( 2 ):
                {
                    if ( Input::pressed( Input::Action::CONFIRM ) )
                    {
                        Audio::playSound( Audio::SoundType::CANCEL );
                        exitNaming();
                    }
                }
                break;
            }
        }
        break;
        case ( State::LOADING ):
        {
            if ( Input::pressed( Input::Action::MOVE_RIGHT ) )
            {
                ++bottom_selection_;
                if ( bottom_selection_ > 3 )
                {
                    bottom_selection_ = 0;
                }
            }
            else if ( Input::pressed( Input::Action::MOVE_LEFT ) )
            {
                --bottom_selection_;
                if ( bottom_selection_ < 0 )
                {
                    bottom_selection_ = 3;
                }
            }
            if ( Input::pressed( Input::Action::CANCEL ) )
            {
                bottom_selection_ = 0;
                state_ = State::SELECT;
            }
            else if ( Input::pressed( Input::Action::CONFIRM ) )
            {
                switch ( bottom_selection_ )
                {
                    case ( 0 ):
                    {
                        Inventory::load( saves_[ selection_ ] );
                        Audio::playSound( Audio::SoundType::CONFIRM );
                        Main::changeState( std::unique_ptr<OverworldState> ( new OverworldState( Inventory::currentSpace() ) ) );
                    }
                    break;
                    case ( 1 ):
                    {
                        bottom_selection_ = 0;
                        name_ = mezun::merge32Strings( saves_[ selection_ ].name(), U" COPY" );
                        Audio::playSound( Audio::SoundType::CONFIRM );
                        state_ = State::COPY;
                    }
                    break;
                    case ( 2 ):
                    {
                        Main::pushState( std::unique_ptr<DeleteSavePromptState> ( new DeleteSavePromptState( saves_[ selection_ ] ) ) );
                        Audio::playSound( Audio::SoundType::CONFIRM );
                        state_ = State::DELETING;
                    }
                    break;
                    case ( 3 ):
                    {
                        bottom_selection_ = 0;
                        Audio::playSound( Audio::SoundType::CANCEL );
                        state_ = State::SELECT;
                    }
                    break;
                }
            }
        }
        break;
    }
    title_.update();
    bg_.update();
};

void ChooseSaveState::stateRender()
{
    Render::colorCanvas( 4 );
    bg_.render();
    switch ( state_ )
    {
        case ( State::SELECT ):
        {
            int i = 0;
            while ( i < saves_.size() )
            {
                const int y = 24 + ( 32 * i );
                const int color = ( selection_ == i ) ? 1 : 3;
                Frame frame { 8, y, 384, 32, color };
                frame.render();
                WTextObj save_name { saves_[ i ].name(), 0, y, WTextCharacter::Color::BLACK, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 16, 8 };
                save_name.render();
                ++i;
            }
            const int y = 24 + ( 32 * i );
            const int color = ( selection_ == i ) ? 1 : 3;
            Frame frame { 8, y, 384, 32, color };
            frame.render();
            WTextObj new_game { U"New Game", 0, y, WTextCharacter::Color::BLACK, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 16, 8 };
            new_game.render();
        }
        break;
        case ( State::NAMING ):
        case ( State::COPY ):
        case ( State::CREATING_NEW_SAVE ):
        case ( State::CREATING_COPY_SAVE ):
        {
            int i = 0;
            while ( i < saves_.size() )
            {
                const int y = 24 + ( 32 * i );
                Frame frame { 8, y, 384, 32, 3 };
                frame.render();
                WTextObj save_name { saves_[ i ].name(), 0, y, WTextCharacter::Color::BLACK, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 16, 8 };
                save_name.render();
                ++i;
            }
            const int y = 24 + ( 32 * i );
            const int new_game_frame_color = ( bottom_selection_ == 0 ) ? 1 : 3;
            Frame new_game_frame { 8, y, 384, 32, new_game_frame_color };
            new_game_frame.render();
            const std::u32string end = ( timer_ > 7 && nameLessThanLimit() ) ? U"_" : U"";
            WTextObj new_game = { mezun::merge32Strings( name_, end ), 0, y, WTextCharacter::Color::BLACK, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 16, 8 };
            new_game.render();

            const int confirm_frame_color = ( bottom_selection_ == 1 ) ? 1 : 3;
            std::u32string confirm_string { U"Confirm" };
            Frame confirm_frame { 8, 184, confirm_string.size() * WTextCharacter::SIZE_PIXELS + 24, 32, confirm_frame_color };
            confirm_frame.render();
            WTextObj confirm { confirm_string, 0, 184, WTextCharacter::Color::BLACK, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 20, 12 };
            confirm.render();

            const int cancel_frame_color = ( bottom_selection_ == 2 ) ? 1 : 3;
            std::u32string cancel_string { U"Cancel" };
            int cancel_frame_w = cancel_string.size() * WTextCharacter::SIZE_PIXELS + 24;
            Frame cancel_frame { Unit::WINDOW_WIDTH_PIXELS - cancel_frame_w - 8, 184, cancel_frame_w, 32, cancel_frame_color };
            cancel_frame.render();
            WTextObj cancel { cancel_string, 0, 184, WTextCharacter::Color::BLACK, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::RIGHT, WTextCharacter::Color::__NULL, 20, 12 };
            cancel.render();

        }
        break;
        case ( State::LOADING ):
        case ( State::DELETING ):
        {
            int i = 0;
            while ( i < saves_.size() )
            {
                const int y = 24 + ( 32 * i );
                const int color = ( selection_ == i ) ? 1 : 3;
                Frame frame { 8, y, 384, 32, color };
                frame.render();
                WTextObj save_name { saves_[ i ].name(), 0, y, WTextCharacter::Color::BLACK, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 16, 8 };
                save_name.render();
                ++i;
            }
            const int y = 24 + ( 32 * i );
            Frame frame { 8, y, 384, 32, 3 };
            frame.render();
            WTextObj new_game { U"New Game", 0, y, WTextCharacter::Color::BLACK, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 16, 8 };
            new_game.render();

            const int play_frame_color = ( bottom_selection_ == 0 ) ? 1 : 3;
            std::u32string play_string { U"Play" };
            Frame play_frame { 8, 184, getBoxSizeFromStringSize( play_string.size() ), 32, play_frame_color };
            play_frame.render();
            WTextObj play { play_string, 0, 184, WTextCharacter::Color::BLACK, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 20, 12 };
            play.render();

            const int cancel_frame_color = ( bottom_selection_ == 3 ) ? 1 : 3;
            std::u32string cancel_string { U"Cancel" };
            int cancel_frame_w = getBoxSizeFromStringSize( cancel_string.size() );
            Frame cancel_frame { Unit::WINDOW_WIDTH_PIXELS - cancel_frame_w - 8, 184, cancel_frame_w, 32, cancel_frame_color };
            cancel_frame.render();
            WTextObj cancel { cancel_string, 0, 184, WTextCharacter::Color::BLACK, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::RIGHT, WTextCharacter::Color::__NULL, 20, 12 };
            cancel.render();

            std::u32string copy_string { U"Copy" };
            std::u32string remove_string { U"Remove" };

            const int max_text_width =
                getBoxSizeFromStringSize( play_string.size() ) +
                getBoxSizeFromStringSize( cancel_string.size() ) +
                getBoxSizeFromStringSize( copy_string.size() ) +
                getBoxSizeFromStringSize( remove_string.size() );

            const int total_space = Unit::WINDOW_WIDTH_PIXELS - max_text_width - 16;
            const int space_size = ( int )( ( double )( total_space ) / 3.0 );

            const int copy_frame_color = ( bottom_selection_ == 1 ) ? 1 : 3;
            int copy_frame_w = getBoxSizeFromStringSize( copy_string.size() );
            Frame copy_frame { 8 + getBoxSizeFromStringSize( play_string.size() ) + space_size, 184, copy_frame_w, 32, copy_frame_color };
            copy_frame.render();
            WTextObj copy { copy_string, getBoxSizeFromStringSize( play_string.size() ) + space_size, 184, WTextCharacter::Color::BLACK, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 20, 12 };
            copy.render();

            const int remove_frame_color = ( bottom_selection_ == 2 ) ? 1 : 3;
            int remove_frame_w = getBoxSizeFromStringSize( remove_string.size() );
            Frame remove_frame { 8 + getBoxSizeFromStringSize( play_string.size() ) + copy_frame_w + ( space_size * 2 ), 184, remove_frame_w, 32, remove_frame_color };
            remove_frame.render();
            WTextObj remove { remove_string, getBoxSizeFromStringSize( play_string.size() ) + copy_frame_w + ( space_size * 2 ), 184, WTextCharacter::Color::BLACK, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 20, 12 };
            remove.render();
        }
        break;
    }
    title_.render();
};

void ChooseSaveState::init()
{
    for ( auto& file : std::filesystem::directory_iterator( Main::saveDirectory() ) )
    {
        // Skip backup saves.
        if ( file.path() == ".sav" || file.path() == "." || file.path() == ".." || mezun::stringEndsWith( file.path(), ".bak" ) )
        {
            continue;
        }

        Save save = Save::loadFromFile( file.path() );
        if ( save.hasError() )
        {
            Main::pushState
            (
                WMessageState::generateErrorMessage
                (
                    mezun::implode( save.getErrors(), mezun::charToChar32String( "\n\n" ) ),
                    WMessageState::Type::POP,
                    nullptr
                )
            );
        }

        if ( !save.isDeleted() )
        {
            saves_.push_back( save );
        }
    }
};

void ChooseSaveState::backFromPop()
{
    switch ( state_ )
    {
        case ( State::DELETING ):
        {
            if ( saves_[ selection_ ].isDeleted() )
            {
                saves_.erase( saves_.begin() + selection_ );
                selection_ = 0;
            }
            state_ = State::SELECT;
        }
        break;
        case ( State::CREATING_NEW_SAVE ):
        case ( State::CREATING_COPY_SAVE ):
        {
            if ( new_save_confirm_ )
            {
                if ( !temp_save_.isDeleted() )
                {
                    saves_.push_back( temp_save_ );
                    temp_save_ = Save::createEmpty();
                    selection_ = saves_.size() - 1;
                }
                state_ = State::SELECT;
            }
            else
            {
                state_ = ( state_ == State::CREATING_NEW_SAVE ) ? State::NAMING : State::COPY;
            }
        }
        break;
    }
};

int ChooseSaveState::maxSelection() const
{
    return saves_.size();
};

bool ChooseSaveState::nameLessThanLimit() const
{
    return name_.size() < NAME_LIMIT;
};

void ChooseSaveState::exitNaming()
{
    Audio::playSound( Audio::SoundType::CANCEL );
    name_ = U"";
    state_ = State::SELECT;
    selection_ = maxSelection();
};

bool ChooseSaveState::testNameAlreadyInUse( const std::u32string& name ) const
{
    for ( const Save& save : saves_ )
    {
        if ( name == save.name() )
        {
            return true;
        }
    }
    return false;
};