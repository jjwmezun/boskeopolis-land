#include "input.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "render.hpp"
#include "shop_state.hpp"
#include "wtext_obj.hpp"

ShopState::ShopState( int shop_number )
:
    GameState(),
    player_ (),
    state_ ( State::WALKING_IN ),
    item_selection_ ( 0 ),
    items_
    (
        { ShopItem::Type::HP_UPGRADE, Localization::getCurrentLanguage().getShopItemHPUpgradeName(), Localization::getCurrentLanguage().getShopItemHPUpgradeDescription() }
    )
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
    }
};

void ShopState::init()
{
    
};