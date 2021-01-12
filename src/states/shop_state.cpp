#include "audio.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "level_list.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "overworld_state.hpp"
#include "render.hpp"
#include "shop_state.hpp"
#include "wtext_obj.hpp"

static constexpr int INPUT_DELAY_LENGTH = 8;
static constexpr int CART_TOTAL_SHOWN_CHANGE = 10000;

static WTextObj generateDialogue( std::u32string text );

ShopState::ShopState( int shop_number )
:
    GameState( StateID::SHOP_STATE, { "Store Brown", 1 } ),
    state_ ( State::WALKING_IN ),
    input_delay_ ( 0 ),
    cart_total_shown_ ( 0 ),
    item_selection_ ( 0 ),
    confirm_selection_ ( 0 ),
    cart_ ( { false, false } ),
    player_ (),
    dialogue_ ( generateDialogue( Localization::getCurrentLanguage().getShopGreeting() ) ),
    item_description_ ( {} ),
    items_
    ({
        { ShopItem::Type::HP_UPGRADE, Localization::getCurrentLanguage().getShopItemHPUpgradeName(), Localization::getCurrentLanguage().getShopItemHPUpgradeDescription(), 250000 },
        { ShopItem::Type::OXYGEN_UPGRADE, Localization::getCurrentLanguage().getShopItemOxygenUpgradeName(), Localization::getCurrentLanguage().getShopItemOxygenUpgradeDescription(), 500000 },
        { ShopItem::Type::SPECIAL_LEVEL_1, Localization::getCurrentLanguage().getUnlockSpecialLevelName( Localization::getCurrentLanguage().getLevelName( LevelList::getCodeNameFromID( LevelList::getSpecialLevelID( 1 ) ) ) ), Localization::getCurrentLanguage().getUnlockSpecialLevelDescription(), 100000 }
    })
{
    Inventory::setSpaceAsShop( shop_number );
    Inventory::save();
};

ShopState::~ShopState() {};

void ShopState::stateUpdate()
{
    switch ( state_ )
    {
        case ( State::WALKING_IN ):
        {
            player_.moveRight();
            if ( player_.testIsAtShopKeeper() )
            {
                state_ = State::CHOOSING;
                setItemDescriptionText();
            }
            if ( Input::pressed( Input::Action::CANCEL ) || Input::pressed( Input::Action::MENU ) )
            {
                state_ = State::LEAVING;
            }
        }
        break;
        case ( State::CHOOSING ):
        {
            if ( Input::pressed( Input::Action::CANCEL ) || Input::pressed( Input::Action::MENU ) )
            {
                changeDialogue( Localization::getCurrentLanguage().getShopLeavingMessage() );
                state_ = State::WAITING_FOR_LEAVING_TEXT_TO_FINISH;
                break;
            }
            else if ( Input::pressed( Input::Action::CONFIRM ) )
            {
                if ( testSelectingCheckout() )
                {
                    if ( testCartIsEmpty() )
                    {
                        Audio::playSound( Audio::SoundType::CANCEL );
                        changeDialogue( Localization::getCurrentLanguage().getNoItemsInCartMessage() );
                    }
                    else if ( calculateCartTotal() > Inventory::totalFunds() )
                    {
                        Audio::playSound( Audio::SoundType::CANCEL );
                        changeDialogue( Localization::getCurrentLanguage().getNotEnoughFundsForCheckoutMessage() );
                    }
                    else
                    {
                        Audio::playSound( Audio::SoundType::CONFIRM );
                        changeDialogue( Localization::getCurrentLanguage().getCheckoutConfirmPrompt() );
                        state_ = State::CONFIRM;
                    }
                }
                else if ( testSelectingExit() )
                {
                    changeDialogue( Localization::getCurrentLanguage().getShopLeavingMessage() );
                    state_ = State::WAITING_FOR_LEAVING_TEXT_TO_FINISH;
                    break;
                }
                else
                {
                    switch ( items_[ item_selection_() ].getState() )
                    {
                        case ( ShopItem::State::AVAILABLE ):
                        {
                            cart_[ item_selection_() ] = !cart_[ item_selection_() ];
                            if ( cart_[ item_selection_() ] )
                            {
                                Audio::playSound( Audio::SoundType::CONFIRM );
                            }
                            else
                            {
                                Audio::playSound( Audio::SoundType::CANCEL );
                            }
                        }
                        break;
                        case ( ShopItem::State::NOT_AVAILABLE ):
                        {
                            Audio::playSound( Audio::SoundType::CANCEL );
                            changeDialogue( Localization::getCurrentLanguage().getShopkeeperNotAvailableMessage() );
                        }
                        break;
                        case ( ShopItem::State::OUT_OF_STOCK ):
                        {
                            Audio::playSound( Audio::SoundType::CANCEL );
                            changeDialogue( Localization::getCurrentLanguage().getShopkeeperOutOfStockMessage() );
                        }
                        break;
                    }
                }
            }
            else if ( input_delay_ == 0 )
            {
                if ( Input::held( Input::Action::MOVE_DOWN ) )
                {
                    ++item_selection_;
                    setItemDescriptionText();
                    Audio::playSound( Audio::SoundType::SELECT );
                    input_delay_ = INPUT_DELAY_LENGTH;
                }
                else if ( Input::held( Input::Action::MOVE_UP ) )
                {
                    --item_selection_;
                    setItemDescriptionText();
                    Audio::playSound( Audio::SoundType::SELECT );
                    input_delay_ = INPUT_DELAY_LENGTH;
                }
            }
            else
            {
                --input_delay_;
            }
            dialogue_.update();
            item_description_.update();

            const int cart_total = calculateCartTotal();
            if ( cart_total_shown_ < cart_total )
            {
                if ( cart_total - cart_total_shown_ > CART_TOTAL_SHOWN_CHANGE )
                {
                    cart_total_shown_ += CART_TOTAL_SHOWN_CHANGE;
                }
                else
                {
                    cart_total_shown_ = cart_total;
                }
            }
            else if ( cart_total_shown_ > cart_total )
            {
                if ( cart_total_shown_ - cart_total > CART_TOTAL_SHOWN_CHANGE )
                {
                    cart_total_shown_ -= CART_TOTAL_SHOWN_CHANGE;
                }
                else
                {
                    cart_total_shown_ = cart_total;
                }
            }
        }
        break;

        case ( State::CONFIRM ):
        {
            if ( Input::pressed( Input::Action::CANCEL ) || Input::pressed( Input::Action::MENU ) )
            {
                Audio::playSound( Audio::SoundType::CANCEL );
                changeDialogue( Localization::getCurrentLanguage().getShopGreeting() );
                state_ = State::CHOOSING;
            }
            else if ( Input::pressed( Input::Action::CONFIRM ) )
            {
                if ( confirm_selection_() == 0 )
                {
                    Inventory::loseTotalFunds( calculateCartTotal() );
                    for ( int i = 0; i < NUMBER_OF_ITEMS; ++i )
                    {
                        if ( cart_[ i ] )
                        {
                            items_[ i ].purchase();
                            cart_[ i ] = false;
                        }
                    }
                    Inventory::save();
                    Audio::playSound( Audio::SoundType::CONFIRM );
                    changeDialogue( Localization::getCurrentLanguage().getCheckoutThanks() );
                    state_ = State::WAITING_FOR_CHECKOUT_THANKS_TO_FINISH;
                }
                else
                {
                    Audio::playSound( Audio::SoundType::CANCEL );
                    changeDialogue( Localization::getCurrentLanguage().getShopGreeting() );
                    state_ = State::CHOOSING;
                }
            }
            else if ( Input::pressed( Input::Action::MOVE_UP ) || Input::pressed( Input::Action::MOVE_DOWN ) )
            {
                Audio::playSound( Audio::SoundType::SELECT );
                ++confirm_selection_;
            }
            dialogue_.update();
            item_description_.update();
        }
        break;

        case ( State::WAITING_FOR_CHECKOUT_THANKS_TO_FINISH ):
        {
            if ( Input::pressed( Input::Action::CANCEL ) || Input::pressed( Input::Action::MENU ) )
            {
                Audio::playSound( Audio::SoundType::CANCEL );
                changeDialogue( Localization::getCurrentLanguage().getShopLeavingMessage() );
                state_ = State::WAITING_FOR_LEAVING_TEXT_TO_FINISH;
                break;
            }
            else if ( dialogue_.finishedAppearing() && Input::pressed( Input::Action::CONFIRM ) )
            {
                Audio::playSound( Audio::SoundType::SELECT );
                changeDialogue( Localization::getCurrentLanguage().getShopGreeting() );
                state_ = State::CHOOSING;
            }
            dialogue_.update();
            item_description_.update();
        }
        break;
        case ( State::WAITING_FOR_LEAVING_TEXT_TO_FINISH ):
        {
            if ( Input::pressed( Input::Action::CANCEL ) || Input::pressed( Input::Action::MENU ) || ( dialogue_.finishedAppearing() && Input::pressed( Input::Action::CONFIRM ) ) )
            {
                Audio::playSound( Audio::SoundType::CANCEL );
                state_ = State::LEAVING;
            }
            dialogue_.update();
            item_description_.update();
        }
        break;
        case ( State::LEAVING ):
        {
            player_.moveLeft();
            if ( player_.hasLeftStore() )
            {
                Audio::playSound( Audio::SoundType::PAGE );
                ShowEventType event_type = ShowEventType::NONE;
                for ( int i = 0; i < NUMBER_OF_ITEMS; ++i )
                {
                    if ( items_[ i ].type() == ShopItem::Type::SPECIAL_LEVEL_1 && items_[ i ].wasPurchased() )
                    {
                        event_type = ShowEventType::NORMAL;
                    }
                }
                Main::changeState( std::unique_ptr<OverworldState> ( new OverworldState( Inventory::currentSpace(), event_type ) ) );
            }
        }
        break;
    }
    Inventory::updateForShop();
    player_.updateGraphics();
};

void ShopState::stateRender()
{
    int number_of_items_in_cart = 0;
    Render::renderObject( "bg/shop-bg.png", { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS }, { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS } );
    player_.render();
    Render::renderObject( "bg/shop-fg.png", { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS }, { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS } );
    if ( state_ == State::CHOOSING || state_ == State::CONFIRM || state_ == State::WAITING_FOR_CHECKOUT_THANKS_TO_FINISH || state_ == State::WAITING_FOR_LEAVING_TEXT_TO_FINISH )
    {
        Render::renderObject( "bg/shop-frames.png", { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS }, { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS } );

        if ( testSelectingCheckout() )
        {
            Render::renderObject( "bg/shop-frame-highlight-3.png", { 0, 0, 140, 19 }, { 14, 127, 140, 19 } );
        }
        else if ( testSelectingExit() )
        {
            Render::renderObject( "bg/shop-frame-highlight-3.png", { 0, 0, 140, 19 }, { 14, 151, 140, 19 } );
        }

        for ( int i = 0; i < NUMBER_OF_ITEMS; ++i )
        {
            const int y = ( i == 0 ) ? 30 : ( 31 + ( i * 24 ) );
            if ( i == 0 )
            {
                if ( item_selection_() == i )
                {
                    Render::renderObject( "bg/shop-frame-highlight.png", { 0, 0, 268, 20 }, { 14, 30, 268, 20 } );
                }
                else if ( items_[ i ].isAvailable() )
                {
                    if ( cart_[ i ] )
                    {
                        Render::renderObject( "bg/shop-frame-selected-1.png", { 0, 0, 268, 20 }, { 14, 30, 268, 20 } );
                    }
                    else
                    {
                        Render::renderObject( "bg/shop-frame-regular-1.png", { 0, 0, 268, 20 }, { 14, 30, 268, 20 } );
                    }
                }
            }
            else
            {
                if ( item_selection_() == i )
                {
                    Render::renderObject( "bg/shop-frame-highlight-2.png", { 0, 0, 268, 19 }, { 14, y, 268, 19 } );
                }
                else if ( items_[ i ].isAvailable() )
                {
                    if ( cart_[ i ] )
                    {
                        Render::renderObject( "bg/shop-frame-selected-2.png", { 0, 0, 268, 19 }, { 14, y, 268, 19 } );
                    }
                    else
                    {
                        Render::renderObject( "bg/shop-frame-regular-2.png", { 0, 0, 268, 19 }, { 14, y, 268, 19 } );
                    }
                }
            }

            items_[ i ].renderMainIcon( 20, y + 2 );
            WTextObj name{ items_[ i ].getName(), 38, y, WTextCharacter::Color::BLACK, 188, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 6, 6, WTextObj::VAlign::CENTER, 20 };
            name.render();

            std::u32string price_text = U"";
            switch ( items_[ i ].getState() )
            {
                case ( ShopItem::State::AVAILABLE ):
                {
                    price_text = mezun::merge32Strings( mezun::charToChar32String( "₧" ), mezun::intToChar32String( items_[ i ].getPrice() ) );
                }
                break;
                case ( ShopItem::State::NOT_AVAILABLE ):
                {
                    price_text = Localization::getCurrentLanguage().getShopNotAvailableText();
                }
                break;
                case ( ShopItem::State::OUT_OF_STOCK ):
                {
                    price_text = Localization::getCurrentLanguage().getShopOutOfStockText();
                }
                break;
            }
            WTextObj price{ price_text, 207, y, WTextCharacter::Color::BLACK, 83, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 6, 6, WTextObj::VAlign::CENTER, 20 };
            price.render();

            if ( cart_[ i ] )
            {
                items_[ i ].renderSmallIcon( 160 + ( number_of_items_in_cart * 8 ), 128 );
                ++number_of_items_in_cart;
            }
        }

        if ( item_selection_() >= 0 )
        {
            item_description_.render();
        }

        WTextObj checkout{ Localization::getCurrentLanguage().getShopCheckout(), 14, 127, WTextCharacter::Color::BLACK, 140, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 6, 6 };
        checkout.render();

        WTextObj exit{ Localization::getCurrentLanguage().getShopExit(), 14, 151, WTextCharacter::Color::BLACK, 140, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 6, 6 };
        exit.render();

        const WTextCharacter::Color color = ( Inventory::totalFundsShown() < 0 ) ? WTextCharacter::Color::LIGHT_MID_GRAY : WTextCharacter::Color::BLACK;
        WTextObj total_points{ mezun::merge32Strings( mezun::charToChar32String( "₧" ), Inventory::totalFundsString() ), 294, 14, color, 386, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 2, 2 };
        total_points.render();
        const WTextCharacter::Color total_cost_color = ( cart_total_shown_ > Inventory::totalFunds() ) ? WTextCharacter::Color::LIGHT_MID_GRAY : WTextCharacter::Color::BLACK;
        WTextObj total_cost{ mezun::merge32Strings( mezun::charToChar32String( "-" ), mezun::intToChar32StringWithPadding( cart_total_shown_, 9 ) ), 304, 40, total_cost_color, 386, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL };
        total_cost.render();

        renderDialogueBox();
        dialogue_.render();

        if ( state_ == State::CONFIRM )
        {
            Render::renderObject( "bg/arrow-icon.png", { 0, 0, 8, 8 }, { 304, 128 + ( confirm_selection_() * 8 ), 8, 8 } );
        }
    }
};

void ShopState::init()
{
    Audio::changeSong( "shop" );
};

bool ShopState::testCartIsEmpty() const
{
    for ( int i = 0; i < NUMBER_OF_ITEMS; ++i )
    {
        if ( cart_[ i ] )
        {
            return false;
        }
    }
    return true;
};

bool ShopState::testSelectingCheckout() const
{
    return item_selection_() == -2;
};

int ShopState::calculateCartTotal() const
{
    int total = 0;
    for ( int i = 0; i < NUMBER_OF_ITEMS; ++i )
    {
        if ( cart_[ i ] )
        {
            total += items_[ i ].getPrice();
        }
    }
    return total;
};

bool ShopState::testSelectingExit() const
{
    return item_selection_() == -1;
};

void ShopState::changeDialogue( const std::u32string& text )
{
    dialogue_ = generateDialogue( text );
};

void ShopState::renderDialogueBox()
{
    const int height = 8 * dialogue_.getText().getNumberOfLines();
    const int y = 138 - height;
    Render::renderObject( "bg/shop-dialogue-frame.png", { 0, 0, 94, 6 }, { 297, y, 94, 6 } );
    Render::renderObject( "bg/shop-dialogue-frame.png", { 0, 6, 94, 1 }, { 297, y + 6, 94, height + 1 } );
    Render::renderObject( "bg/shop-dialogue-frame.png", { 0, 7, 94, 20 }, { 297, 145, 94, 20 } );
};

void ShopState::setItemDescriptionText()
{
    std::u32string text = U"";
    if ( item_selection_() >= 0 )
    {
        text = items_[ item_selection_() ].getDescription();
    }
    item_description_ = { { text, 6, 190, WTextCharacter::Color::BLACK, 388, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 6, 6, WTextObj::VAlign::TOP, 28 } };
};

static WTextObj generateDialogue( std::u32string text )
{
    return { text, 304, 72, WTextCharacter::Color::BLACK, 72, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 0, 0, WTextObj::VAlign::BOTTOM, 72 };
};