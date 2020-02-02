#include "input.hpp"
#include "inventory.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "render.hpp"
#include "shop_state.hpp"
#include "wtext_obj.hpp"

static constexpr int INPUT_DELAY_LENGTH = 8;

ShopState::ShopState( int shop_number )
:
    GameState(),
    player_ (),
    state_ ( State::WALKING_IN ),
    item_selection_ ( 0 ),
    items_
    ({
        { ShopItem::Type::HP_UPGRADE, Localization::getCurrentLanguage().getShopItemHPUpgradeName(), Localization::getCurrentLanguage().getShopItemHPUpgradeDescription(), 500000 },
        { ShopItem::Type::OXYGEN_UPGRADE, Localization::getCurrentLanguage().getShopItemOxygenUpgradeName(), Localization::getCurrentLanguage().getShopItemOxygenUpgradeDescription(), 1000000 }
    }),
    input_delay_ ( 0 ),
    cart_ ()
{};

ShopState::~ShopState() {};

void ShopState::stateUpdate()
{
    player_.update();
    switch ( state_ )
    {
        case ( State::WALKING_IN ):
        {
            if ( !player_.testIsWalking() )
            {
                state_ = State::CHOOSING;
            }
        }
        break;
        case ( State::CHOOSING ):
        {
            if ( input_delay_ == 0 )
            {
                if ( Input::held( Input::Action::MOVE_DOWN ) )
                {
                    ++item_selection_;
                    input_delay_ = INPUT_DELAY_LENGTH;
                }
                else if ( Input::held( Input::Action::MOVE_UP ) )
                {
                    --item_selection_;
                    input_delay_ = INPUT_DELAY_LENGTH;
                }
            }
            else
            {
                --input_delay_;
            }
        }
        break;
    }
    if ( Input::held( Input::Action::CANCEL ) )
    {
        Main::popState( true );
    }
};

void ShopState::stateRender()
{
    Render::renderObject( "bg/shop-bg.png", { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS }, { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS } );
    player_.render();
    Render::renderObject( "bg/shop-fg.png", { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS }, { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS } );
    if ( state_ == State::CHOOSING )
    {
        Render::renderObject( "bg/shop-frames.png", { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS }, { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS } );
        if ( item_selection_() == 0 )
        {
            Render::renderObject( "bg/shop-frame-highlight.png", { 0, 0, 268, 20 }, { 14, 30, 268, 20 } );
        }
        else if ( item_selection_() < 0 )
        {
            Render::renderObject( "bg/shop-frame-highlight-3.png", { 0, 0, 140, 19 }, { 14, 151, 140, 19 } );
        }
        else
        {
            Render::renderObject( "bg/shop-frame-highlight-2.png", { 0, 0, 268, 19 }, { 14, 55, 268, 19 } );
        }

        for ( int i = 0; i < NUMBER_OF_ITEMS; ++i )
        {
            const int y = ( i == 0 ) ? 30 : 55;
            Render::renderObject( "bg/shop-items.png", { 0, 0, 16, 16  }, { 20, y + 2, 16, 16 } );
            WTextObj name{ items_[ i ].getName(), 38, y, WTextCharacter::Color::BLACK, 188, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 6, 6 };
            name.render();
            WTextObj price{ mezun::merge32Strings( mezun::charToChar32String( "₧" ), mezun::intToChar32String( items_[ i ].getPrice() ) ), 207, y, WTextCharacter::Color::BLACK, 83, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 6, 6 };
            price.render();
        }

        if ( item_selection_() >= 0 )
        {
            WTextObj desc{ items_[ item_selection_() ].getDescription(), 6, 190, WTextCharacter::Color::BLACK, 388, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 6, 6, WTextObj::VAlign::TOP, 28 };
            desc.render();
        }

        WTextObj checkout{ Localization::getCurrentLanguage().getShopCheckout(), 14, 151, WTextCharacter::Color::BLACK, 140, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 6, 6 };
        checkout.render();

        WTextCharacter::Color color = ( Inventory::totalFundsShown() < 0 ) ? WTextCharacter::Color::LIGHT_MID_GRAY : WTextCharacter::Color::BLACK;
        WTextObj total_points{ mezun::merge32Strings( mezun::charToChar32String( "₧" ), Inventory::totalFundsString() ), 294, 14, color, 386, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 2, 2 };
        total_points.render();
    }
};

void ShopState::init()
{
    
};