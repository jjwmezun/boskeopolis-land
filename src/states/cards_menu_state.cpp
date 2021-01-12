#include "audio.hpp"
#include "cards_menu_state.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "level_list.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "render.hpp"

static constexpr int CARD_HEIGHT = 29;
static constexpr int CARD_WIDTH = 20;
static constexpr int CARD_PADDING = 4;
static constexpr int MESSAGE_HEIGHT = 64;
static constexpr int INPUT_DELAY_LENGTH = 8;

static constexpr int generateCardX( int x )
{
    return 10 + ( CARD_WIDTH + CARD_PADDING ) * x;
};

static constexpr int generateCardY( int y )
{
    return ( CARD_HEIGHT + CARD_PADDING ) * y + 24;
};

CardsMenuState::CardsMenuState()
:
    GameState( StateID::CARD_MENU_STATE, { "Triste Blue", 1 }),
    message_frame_ ( 0, Unit::WINDOW_HEIGHT_PIXELS - MESSAGE_HEIGHT, Unit::WINDOW_WIDTH_PIXELS, MESSAGE_HEIGHT ),
    title_
    (
        { mezun::charToChar32String( "Trading Cards" ), 0, 0, WTextCharacter::Color::LIGHT_GRAY, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::CENTER, WTextCharacter::Color::BLACK, 8, 8 }
    ),
    selection_x_ ( 0 ),
    selection_y_ ( 0 ),
    bg_ (),
    input_delay_ ( INPUT_DELAY_LENGTH )
{
    for ( int cycle = 0; cycle < Level::NUMBER_OF_CYCLES; ++cycle )
    {
        for ( int theme = 0; theme < Level::NUMBER_OF_THEMES; ++theme )
        {
            const TradingCard& card = Localization::getCurrentLanguage().getLevelTradingCard( LevelList::getCodeNameByCycleAndTheme( cycle, theme ) );
            card_text_[ cycle ][ theme ] =
            {
                mezun::merge32Strings( mezun::merge32Strings( card.name_, U"\n" ), card.description_ ),
                40,
                Unit::WINDOW_HEIGHT_PIXELS - MESSAGE_HEIGHT,
                WTextCharacter::Color::BLACK,
                Unit::WINDOW_WIDTH_PIXELS - 40,
                WTextObj::Align::LEFT,
                WTextCharacter::Color::__NULL,
                8,
                8,
                WTextObj::VAlign::TOP,
                MESSAGE_HEIGHT
            };
        }
    }
};

CardsMenuState::~CardsMenuState() {};

void CardsMenuState::stateUpdate()
{
	if ( Input::pressed( Input::Action::MENU ) )
	{
		Audio::playSound( Audio::SoundType::CANCEL );
		Main::popState( true );
	}
    title_.update();

    if ( input_delay_ == INPUT_DELAY_LENGTH )
    {
        if ( Input::held( Input::Action::MOVE_RIGHT ) )
        {
            ++selection_x_;
            doGeneralSelection();
        }
        else if ( Input::held( Input::Action::MOVE_LEFT ) )
        {
            --selection_x_;
            doGeneralSelection();
        }

        if ( Input::held( Input::Action::MOVE_DOWN ) )
        {
            ++selection_y_;
            doGeneralSelection();
        }
        else if ( Input::held( Input::Action::MOVE_UP ) )
        {
            --selection_y_;
            doGeneralSelection();
        }
    }
    else
    {
        ++input_delay_;
    }

    bg_.update();
};

void CardsMenuState::stateRender()
{
    bg_.render();
    message_frame_.render();
    for ( int yi = 0; yi < Level::NUMBER_OF_CYCLES; ++yi )
    {
        for ( int xi = 0; xi < Level::NUMBER_OF_THEMES; ++xi )
        {
            const int x = generateCardX( xi );
            const int y = generateCardY( yi );
            Render::renderObject( "bg/card-select-card-2.png", { 0, 0, CARD_WIDTH, CARD_HEIGHT }, { x, y, CARD_WIDTH, CARD_HEIGHT } );
            Render::renderObject( "bg/card-select-card-2.png", { 40, 0, 13, 22 }, { x + 4, y + 4, 13, 22 } );
            if ( Inventory::haveDiamond( LevelList::getIDbyCycleAndTheme( yi, xi ) ) )
            {
                Render::renderRect( { x + 4, y + 4, 13, 22 }, 1 );
                Render::renderObject( "bg/trading-card-items-small.png", { 13 * xi, 22 * yi, 13, 22 }, { x + 4, y + 4, 13, 22 } );
            }
        }
    }
    Render::renderObject( "bg/card-select-card-2.png", { 20, 0, CARD_WIDTH, CARD_HEIGHT }, { generateCardX( selection_x_.value() ), generateCardY( selection_y_.value() ), CARD_WIDTH, CARD_HEIGHT } );

    if ( Inventory::haveDiamond( LevelList::getIDbyCycleAndTheme( selection_y_.value(), selection_x_.value() ) ) )
    {
        Render::renderObject( "bg/trading-card-items-large.png", { 39 * selection_x_.value(), 48 * selection_y_.value(), 39, 48 }, { 8, 168, 39, 48 } );
        card_text_[ selection_y_.value() ][ selection_x_.value() ].render();
    }
    title_.render();
};

void CardsMenuState::init()
{
    Audio::changeSong( "level-select" );
};

void CardsMenuState::doGeneralSelection()
{
    Audio::playSound( Audio::SoundType::SELECT );
    input_delay_ = 0;
};