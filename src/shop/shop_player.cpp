#include "render.hpp"
#include "shop_player.hpp"

ShopPlayer::ShopPlayer()
:
    animation_timer_ (),
    walk_counter_ (),
    blink_counter_ (),
    graphics_ ( "sprites/autumn.png", { 0, 0, 16, 26 }, { -32, 176, 16, 26 } ),
    position_ ( -32.0 ),
    speed_ ( 2.0 )
{};

void ShopPlayer::update()
{
    updatePosition();
    updateGraphics();
};

void ShopPlayer::render() const
{
    Render::renderObject( graphics_.image_, graphics_.src_, graphics_.dest_, SDL_FLIP_HORIZONTAL );
};

void ShopPlayer::updatePosition()
{
    if ( speed_ > 0.000001 )
    {
        if ( position_ > 250.0 )
        {
            speed_ /= 1.2;
        }
        position_ += speed_;
        graphics_.dest_.x = ( int )( position_ );
    }
};

void ShopPlayer::updateGraphics()
{
    if ( isMoving() )
	{
		if ( animation_timer_.update() )
		{
			++walk_counter_;
		}
        graphics_.src_.x = WALK_FRAMES[  walk_counter_.value() ];
	}
	else
	{
		if ( animation_timer_.update() )
		{
			++blink_counter_;
		}
		graphics_.src_.x = ( blink_counter_.value() == 3 ) ? 112 : 80;
	}
};

bool ShopPlayer::isMoving() const
{
    return speed_ > 1.999999;
};