#include "inventory.hpp"
#include "render.hpp"
#include "shop_player.hpp"

static constexpr double START_SPEED = 4.0;
static constexpr double START_POSITION = -32.0;
static constexpr double MIN_MOVING_SPEED = 1.999999;

ShopPlayer::ShopPlayer()
:
    animation_timer_ (),
    walk_counter_ (),
    blink_counter_ (),
    graphics_ ( Inventory::getPlayerCostume(), { 0, 78, 29, 26 }, { -32, 156, 29, 26 } ),
    position_ ( START_POSITION ),
    speed_ ( START_SPEED ),
    flip_ ( SDL_FLIP_HORIZONTAL )
{};

void ShopPlayer::moveRight()
{
    if ( testIsWalking() )
    {
        if ( position_ > 250.0 )
        {
            speed_ /= 1.2;
        }
        position_ += speed_;
        graphics_.dest_.x = ( int )( position_ );
    }
};

void ShopPlayer::moveLeft()
{
    speed_ = -START_SPEED;
    position_ += speed_;
    graphics_.dest_.x = ( int )( position_ );
    flip_ = SDL_FLIP_NONE;
};

void ShopPlayer::render() const
{
    Render::renderObject( graphics_.image_, graphics_.src_, graphics_.dest_, flip_ );
};

void ShopPlayer::updateGraphics()
{
    if ( isMoving() )
	{
		if ( animation_timer_.update() )
		{
			++walk_counter_;
		}
        graphics_.src_.x = WALK_FRAMES[ walk_counter_.value() ];
	}
	else
	{
		if ( animation_timer_.update() )
		{
			++blink_counter_;
		}
		graphics_.src_.x = ( blink_counter_.value() == 3 ) ? 87 : 0;
	}
};

bool ShopPlayer::isMoving() const
{
    return speed_ > MIN_MOVING_SPEED || speed_ < -MIN_MOVING_SPEED;
};

bool ShopPlayer::testIsWalking() const
{
    return speed_ > 0.1;
};

bool ShopPlayer::testIsAtShopKeeper() const
{
    return !testIsWalking();
};

bool ShopPlayer::hasLeftStore() const
{
    return position_ < START_POSITION;
};