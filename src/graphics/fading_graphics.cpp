#include "fading_graphics.hpp"

#define FULL_OPACITY 255
#define changePerUpdate( speed ) floor( FULL_OPACITY / ( speed ) )

FadingGraphics::FadingGraphics
(
	std::string&& texture,
	int frame_x,
	int frame_y,
	bool start_on,
	int speed,
	bool flip_x,
	bool flip_y,
	double rotation,
	int x_adjustment,
	int y_adjustment,
	int w_adjustment,
	int h_adjustment,
	int animation_speed
)
:
	SpriteGraphics( std::forward<std::string> ( texture ), frame_x, frame_y, flip_x, flip_y, rotation, x_adjustment, y_adjustment, w_adjustment, h_adjustment, ( ( start_on ) ? FULL_OPACITY : 0 ) ),
	change_per_update_ ( changePerUpdate( speed ) ),
	fading_in_ ( !start_on )
{};

FadingGraphics::~FadingGraphics() {};

void FadingGraphics::update()
{
	if ( fading_in_ )
	{
		alpha_ = std::min( FULL_OPACITY, ( int )( alpha_ ) + change_per_update_ );
		fading_in_ = ( alpha_ != FULL_OPACITY );
	}
	else
	{
		alpha_ = std::max( 0, ( int )( alpha_ ) - change_per_update_ );
		fading_in_ = ( alpha_ == 0 );
	}
};