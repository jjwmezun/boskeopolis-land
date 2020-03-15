#include "scrolling_background.hpp"
#include "render.hpp"
#include "unit.hpp"

ScrollingBackground::ScrollingBackground( std::string texture, int width, int height, int speed )
:
    timer_ ( 0 ),
    speed_ ( speed ),
    width_ ( width ),
    height_ ( height ),
	graphics_
    (
        texture,
        { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS },
	    { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS }
    )
{};

void ScrollingBackground::update()
{
	if ( timer_ == speed_ )
	{
		timer_ = 0;
		++graphics_.src_.x;
		if ( graphics_.src_.x == width_ )
		{
			graphics_.src_.x = 0;
		}
		++graphics_.src_.y;
		if ( graphics_.src_.y == height_ )
		{
			graphics_.src_.y = 0;
		}
	}
	else
	{
		++timer_;
	}
};

void ScrollingBackground::render() const
{
    graphics_.render();
};