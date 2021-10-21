#include "audio.hpp"
#include "cards_menu_state.hpp"
#include "inventory.hpp"
#include "level_list.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "render.hpp"
#include "wtext_obj.hpp"

static constexpr int CARD_HEIGHT = 29;
static constexpr int CARD_WIDTH = 20;
static constexpr int CARD_PADDING = 4;
static constexpr int MESSAGE_HEIGHT = 136;
static constexpr int MAX_DELAY = 8;
static constexpr int SCROLL_SPEED = 16;
static constexpr int FRAME_Y = Unit::WINDOW_HEIGHT_PIXELS - MESSAGE_HEIGHT;
static constexpr int BIG_IMAGE_Y = FRAME_Y + 8;

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
    message_frame_ ( 0, FRAME_Y, Unit::WINDOW_WIDTH_PIXELS, MESSAGE_HEIGHT ),
    title_
    (
        { mezun::charToChar32String( "Trading Cards" ), 0, 0, WTextCharacter::Color::LIGHT_GRAY, Unit::WINDOW_WIDTH_PIXELS, WTextObj::Align::CENTER, WTextCharacter::Color::BLACK, 8, 8 }
    ),
    selection_x_ ( 0 ),
    selection_y_ ( 0 ),
    selection_ ( 0 ),
    current_page_ ( 0 ),
    next_page_ ( 0 ),
    cols_on_last_page_ ( 0 ),
    rows_on_last_page_ ( 0 ),
    arrow_timer_ ( 0 ),
    bg_ (),
    input_delay_ ( MAX_DELAY ),
    card_pages_ ({}),
    pages_gfx_ ( Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS ),
    prev_selection_ ( 0 ),
    prev_input_ ( Input::Action::CANCEL ),
    move_timer_ ( 0 )
{
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
            card_pages_.push_back({});
            ++page;
        }

        const TradingCard& card = Localization::getCurrentLanguage().getLevelTradingCard( LevelList::getCodeNameFromID( i ) );

        card_pages_.at( page - 1 ).push_back
        ({
            i,
            { WTextObj
            {
                mezun::merge32Strings
                (
                    mezun::merge32Strings
                    (
                        mezun::merge32Strings
                        (
                            card.name_, U"\n\n"
                        ),
                        mezun::merge32Strings
                        (
                            card.description_, U"\n\n"
                        )
                    ),
                    mezun::merge32Strings
                    (
                        U"Episode: ",
                        card.episode_
                    )
                ),
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
        }});
        ++cardnum;
    }

    pages_gfx_.setWidth( Unit::WINDOW_WIDTH_PIXELS * card_pages_.size() );

    const auto& last_page = card_pages_.at( card_pages_.size() - 1 );
    cols_on_last_page_ = last_page.size() % CARDS_PER_COLUMN;
    if ( cols_on_last_page_ == 0 )
    {
        cols_on_last_page_ = CARDS_PER_COLUMN;
    }
    rows_on_last_page_ = ( int )( std::ceil( ( double )( last_page.size() ) / ( double )( CARDS_PER_COLUMN ) ) );
};

CardsMenuState::~CardsMenuState()
{
    pages_gfx_.destroy();
};

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
        const int delay_length = ( move_timer_ >= 8 ) ? 2 : ( ( move_timer_ >= 4 ) ? 4 : MAX_DELAY );
        Input::Action current_input = Input::Action::CANCEL;
        if ( input_delay_ >= delay_length )
        {
            if ( Input::held( Input::Action::MOVE_RIGHT ) )
            {
                ++selection_x_;
                if ( selection_x_ == CARDS_PER_COLUMN )
                {
                    if ( current_page_ < card_pages_.size() - 1 )
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
                current_input = Input::Action::MOVE_RIGHT;
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
                current_input = Input::Action::MOVE_LEFT;
            }
            else
            {
                current_input = Input::Action::CANCEL;
            }

            ++move_timer_;
            if ( current_input != prev_input_ || current_input == Input::Action::CANCEL )
            {
                move_timer_ = 0;
            }
            prev_input_ = current_input;

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

            if ( next_page_ == card_pages_.size() - 1 )
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
            prev_selection_ = selection_;
            selection_ = ( CARDS_PER_COLUMN * selection_y_ ) + selection_x_;
            if ( prev_selection_ != selection_ )
            {
                card_pages_[ current_page_ ][ selection_ ].text.reset();
            }
        }
        else
        {
            ++input_delay_;
        }
    }

    card_pages_[ current_page_ ][ selection_ ].text.update();
    bg_.update();
    ++arrow_timer_;
    if ( arrow_timer_ >= 32 )
    {
        arrow_timer_ = 0;
    }
};

void CardsMenuState::stateRender()
{
    bg_.render();
    message_frame_.render();
    pages_gfx_.render();
    if ( next_page_ == current_page_ )
    {
        // Render select highlight.
        Render::renderObject( "bg/card-select-card-2.png", { 20, 0, CARD_WIDTH, CARD_HEIGHT }, { generateCardX( selection_x_ ), generateCardY( selection_y_ ), CARD_WIDTH, CARD_HEIGHT } );

        // If selected card has diamond.
        if ( Inventory::haveDiamond( card_pages_[ current_page_ ][ selection_ ].id ) )
        {
            // Render card text.
            if ( selection_ < card_pages_[ current_page_ ].size() )
            {
                card_pages_[ current_page_ ][ selection_ ].text.render();
                
            }

            // Render card image.
            Render::renderObject( "bg/trading-card-items-large.png", { 39 * selection_x_, 48 * selection_y_, 39, 48 }, { 8, BIG_IMAGE_Y, 39, 48 } );
        }

        // Render arrows.
        if ( arrow_timer_ > 15 )
        {
            if ( current_page_ > 0 )
            {
                Render::renderObject( "bg/level-select-characters.png", { 16, 16, 16, 16 }, { 0, 50, 16, 16 } );
            }

            if ( current_page_ < card_pages_.size() - 1 )
            {
                Render::renderObject( "bg/level-select-characters.png", { 16, 16, 16, 16 }, { Unit::WINDOW_WIDTH_PIXELS - 16, 50, 16, 16 }, SDL_FLIP_HORIZONTAL );
            }
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
    for ( int li = 0; li < card_pages_.size(); ++li )
    {
        for ( int i = 0; i < card_pages_[ li ].size(); ++i )
        {
            const int xi = mezun::xOfN( i, CARDS_PER_COLUMN );
            const int yi = mezun::yOfN( i, CARDS_PER_COLUMN );
            const int x = generateCardX( xi ) + Unit::WINDOW_WIDTH_PIXELS * li;
            const int y = generateCardY( yi );
            Render::renderObject( "bg/card-select-card-2.png", src, { x, y, CARD_WIDTH, CARD_HEIGHT } );
            Render::renderObject( "bg/card-select-card-2.png", { 40, 0, 13, 22 }, { x + 4, y + 4, 13, 22 } );
            if ( Inventory::haveDiamond( card_pages_[ li ][ i ].id ) )
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