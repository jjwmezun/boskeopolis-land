#include "audio.hpp"
#include <filesystem>
#include "frame.hpp"
#include "choose_save_state.hpp"
#include "counter_t.hpp"
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

ChooseSaveState::ChooseSaveState()
:
    GameState( StateID::LOAD_GAME_STATE ),
    saves_ (),
    selection_ ( 0 ),
    state_ ( State::SELECT ),
    name_ (),
    timer_ ( 0 )
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
                    selection_ = 0;
                }
                else
                {
                    Inventory::load( saves_[ selection_ ] );
                    Main::changeState( std::unique_ptr<OverworldState> ( new OverworldState( 0 ) ) );
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
        {
            if ( Input::pressed( Input::Action::MOVE_DOWN ) || Input::pressed( Input::Action::MOVE_UP ) )
            {
                Audio::playSound( Audio::SoundType::SELECT );
                selection_ = ( selection_ == 0 ) ? 1 : 0;
            }
            else if ( selection_ > 0 && ( Input::pressed( Input::Action::MOVE_LEFT ) || Input::pressed( Input::Action::MOVE_RIGHT ) ) )
            {
                Audio::playSound( Audio::SoundType::SELECT );
                selection_ = ( selection_ == 1 ) ? 2 : 1;
            }

            switch ( selection_ )
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
                        if ( !name_.empty() )
                        {
                            Main::pushState( std::unique_ptr<NewGameConfirmPromptState> ( new NewGameConfirmPromptState( name_ ) ) );
                            Audio::playSound( Audio::SoundType::CONFIRM );
                        }
                        else
                        {
                            Audio::playSound( Audio::SoundType::CANCEL );
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
                        exitNaming();
                    }
                }
                break;
            }
        }
        break;
    }
};

void ChooseSaveState::stateRender()
{
    Render::colorCanvas( 4 );
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
        {
            int i = 0;
            while ( i < saves_.size() )
            {
                const int y = 24 + ( 32 * i );
                Frame frame { 8, y, 384, 32, 3 };
                frame.render();
                ++i;
            }
            const int y = 24 + ( 32 * i );
            const int new_game_frame_color = ( selection_ == 0 ) ? 1 : 3;
            Frame new_game_frame { 8, y, 384, 32, new_game_frame_color };
            new_game_frame.render();
            const std::u32string end = ( timer_ > 7 && nameLessThanLimit() ) ? U"_" : U"";
            WTextObj new_game = { mezun::merge32Strings( name_, end ), 0, y, WTextCharacter::Color::BLACK, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 16, 8 };
            new_game.render();

            const int confirm_frame_color = ( selection_ == 1 ) ? 1 : 3;
            std::u32string confirm_string { U"Confirm" };
            Frame confirm_frame { 8, 184, confirm_string.size() * WTextCharacter::SIZE_PIXELS + 24, 32, confirm_frame_color };
            confirm_frame.render();
            WTextObj confirm { confirm_string, 0, 184, WTextCharacter::Color::BLACK, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 20, 12 };
            confirm.render();

            const int cancel_frame_color = ( selection_ == 2 ) ? 1 : 3;
            std::u32string cancel_string { U"Cancel" };
            int cancel_frame_w = cancel_string.size() * WTextCharacter::SIZE_PIXELS + 24;
            Frame cancel_frame { Unit::WINDOW_WIDTH_PIXELS - cancel_frame_w - 8, 184, cancel_frame_w, 32, cancel_frame_color };
            cancel_frame.render();
            WTextObj cancel { cancel_string, 0, 184, WTextCharacter::Color::BLACK, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::RIGHT, WTextCharacter::Color::__NULL, 20, 12 };
            cancel.render();

        }
        break;
    }
};

void ChooseSaveState::init()
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