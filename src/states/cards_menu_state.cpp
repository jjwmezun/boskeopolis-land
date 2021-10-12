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
static constexpr int SCROLL_SPEED = 8;

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
    input_delay_ ( INPUT_DELAY_LENGTH ),
    selection_ ( 0 ),
    pages_ ( ( int )( std::ceil( ( double )( LevelList::getNumberOfLevels() ) / ( double )( CARDS_PER_PAGE ) ) ) ),
    current_page_ ( 0 ),
    cards_ ({}),
    src_ ( 0, 0, CARD_WIDTH, CARD_HEIGHT ),
    cols_on_last_page_ ( 0 ),
    rows_on_last_page_ ( 0 ),
    pages_gfx_ ( Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS ),
    next_page_ ( 0 )
{
    /*
    for ( int i = 0; i < LevelList::getNumberOfLevels(); ++i )
    {
        if ( LevelList::hasCard( i ) )
        {
            const TradingCard& card = Localization::getCurrentLanguage().getLevelTradingCard( LevelList::getCodeNameFromID( i ) );
            card_text_.emplace_back
            (
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
            );
            card_level_ids_.emplace_back( i );
        }
    }
    number_of_rows_ = ;*/

    int page = 0;
    int cardnum = 0;
    for ( int i = 0; i < LevelList::getNumberOfLevels(); ++i )
    {
        // Skip levels without
        if ( !LevelList::hasCard( i ) )
        {
            continue;
        }

        if ( cardnum % CARDS_PER_PAGE == 0 )
        {
            cardnum = 0;
            cards_.push_back({});
            ++page;
        }

        const TradingCard& card = Localization::getCurrentLanguage().getLevelTradingCard( LevelList::getCodeNameFromID( i ) );

        cards_.at( page - 1 ).push_back
        ({
            i,
            WTextObj
            {
                mezun::merge32Strings( mezun::merge32Strings( mezun::merge32Strings( card.name_, U"\n" ), mezun::merge32Strings( card.description_, U"\n" ) ), card.episode_ ),
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
            }
        });
        ++cardnum;
    }

    pages_gfx_.setWidth( Unit::WINDOW_WIDTH_PIXELS * cards_.size() );

    const auto& last_page = cards_.at( cards_.size() - 1 );
    cols_on_last_page_ = last_page.size() % CARDS_PER_COLUMN;
    if ( cols_on_last_page_ == 0 )
    {
        cols_on_last_page_ = CARDS_PER_COLUMN;
    }
    rows_on_last_page_ = ( int )( std::ceil( ( double )( last_page.size() ) / ( double )( CARDS_PER_COLUMN ) ) );
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

    if ( next_page_ > current_page_ )
    {
        if ( pages_gfx_.getX() <= next_page_ * -Unit::WINDOW_WIDTH_PIXELS )
        {
            current_page_ = next_page_;
        }
        else
        {
            pages_gfx_.setX( pages_gfx_.getX() - SCROLL_SPEED );
        }
    }
    else if ( next_page_ < current_page_ )
    {
        if ( pages_gfx_.getX() >= next_page_ * -Unit::WINDOW_WIDTH_PIXELS )
        {
            current_page_ = next_page_;
        }
        else
        {
            pages_gfx_.setX( pages_gfx_.getX() + SCROLL_SPEED );
        }
    }
    else
    {
        if ( input_delay_ == INPUT_DELAY_LENGTH )
        {
            if ( Input::held( Input::Action::MOVE_RIGHT ) )
            {
                ++selection_x_;
                if ( selection_x_ == CARDS_PER_COLUMN )
                {
                    if ( current_page_ < cards_.size() - 1 )
                    {
                        selection_x_ = 0;
                        next_page_ = current_page_ + 1;
                    }
                    else
                    {
                        selection_x_ = CARDS_PER_COLUMN - 1;
                    }
                }
                doGeneralSelection();
            }
            else if ( Input::held( Input::Action::MOVE_LEFT ) )
            {
                --selection_x_;
                if ( selection_x_ < 0 )
                {
                    if ( current_page_ > 0 )
                    {
                        selection_x_ = CARDS_PER_COLUMN - 1;
                        next_page_ = current_page_ - 1;
                    }
                    else
                    {
                        selection_x_ = 0;
                    }
                }
                doGeneralSelection();
            }

            if ( Input::held( Input::Action::MOVE_DOWN ) )
            {
                ++selection_y_;
                if ( selection_y_ == CARDS_PER_ROW )
                {
                    selection_y_ = CARDS_PER_ROW - 1;
                }
                doGeneralSelection();
            }
            else if ( Input::held( Input::Action::MOVE_UP ) )
            {
                --selection_y_;
                if ( selection_y_ < 0 )
                {
                    selection_y_ = 0;
                }
                doGeneralSelection();
            }

            if ( next_page_ == cards_.size() - 1 )
            {
                if ( selection_y_ >= rows_on_last_page_ )
                {
                    selection_y_ = rows_on_last_page_ - 1;
                }
                else if ( selection_y_ == rows_on_last_page_ - 1 && selection_x_ >= cols_on_last_page_ )
                {
                    selection_x_ = cols_on_last_page_ - 1;
                }
            }
            selection_ = ( CARDS_PER_COLUMN * selection_y_ ) + selection_x_;
        }
        else
        {
            ++input_delay_;
        }
    }

    bg_.update();
};

void CardsMenuState::stateRender()
{
    bg_.render();
    message_frame_.render();
    pages_gfx_.render();
    if ( next_page_ == current_page_ )
    {
        Render::renderObject( "bg/card-select-card-2.png", { 20, 0, CARD_WIDTH, CARD_HEIGHT }, { generateCardX( selection_x_ ), generateCardY( selection_y_ ), CARD_WIDTH, CARD_HEIGHT } );
        if ( selection_ < cards_[ current_page_ ].size() )
        {
            cards_[ current_page_ ][ selection_ ].text.render();
        }
    }
    title_.render();
};

void CardsMenuState::init()
{
    Audio::changeSong( "level-select" );

    pages_gfx_.init();
    pages_gfx_.startDrawing();
    sdl2::SDLRect src = { 0, 0, CARD_WIDTH, CARD_HEIGHT };
    for ( int li = 0; li < cards_.size(); ++li )
    {
        for ( int i = 0; i < cards_[ li ].size(); ++i )
        {
            const int xi = mezun::xOfN( i, CARDS_PER_COLUMN );
            const int yi = mezun::yOfN( i, CARDS_PER_COLUMN );
            const int x = generateCardX( xi ) + Unit::WINDOW_WIDTH_PIXELS * li;
            const int y = generateCardY( yi );
            Render::renderObject( "bg/card-select-card-2.png", src, { x, y, CARD_WIDTH, CARD_HEIGHT } );
            Render::renderObject( "bg/card-select-card-2.png", { 40, 0, 13, 22 }, { x + 4, y + 4, 13, 22 } );
            if ( Inventory::haveDiamond( cards_[ li ][ i ].id ) )
            {
                Render::renderRect( { x + 4, y + 4, 13, 22 }, 1 );
                Render::renderObject( "bg/trading-card-items-small.png", { 13 * xi, 22 * yi, 13, 22 }, { x + 4, y + 4, 13, 22 } );
            }
        }
    }
    pages_gfx_.endDrawing();
};

void CardsMenuState::doGeneralSelection()
{
    Audio::playSound( Audio::SoundType::SELECT );
    input_delay_ = 0;
};