#include "input.hpp"
#include "main.hpp"
#include "render.hpp"
#include "shop_state.hpp"

ShopState::ShopState( int shop_number )
:
    GameState(),
    player_ ()
{};

ShopState::~ShopState() {};

void ShopState::stateUpdate()
{
    player_.update();
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
};

void ShopState::init()
{
    
};