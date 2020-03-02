#include "checkered_background.hpp"
#include "render.hpp"
#include "unit.hpp"

static constexpr int BACKGROUND_SPEED = 4;
static constexpr int BACKGROUND_TILE_SIZE = 32;

CheckeredBackground::CheckeredBackground()
:
    timer_ ( 0 ),
	screen_ ( 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS ),
	position_ ( 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS )
{};

void CheckeredBackground::update()
{
	if ( timer_ == BACKGROUND_SPEED )
	{
		timer_ = 0;
		++position_.x;
		if ( position_.x == BACKGROUND_TILE_SIZE )
		{
			position_.x = 0;
		}
		// Since vertical & horizontal movement are the same, y can always be the same as x.
		position_.y = position_.x;
	}
	else
	{
		++timer_;
	}
};

void CheckeredBackground::render() const
{
	Render::renderObject( "bg/level-select-back.png", position_, screen_ );
};