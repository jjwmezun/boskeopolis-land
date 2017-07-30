#include "animated_graphics.hpp"

AnimatedGraphics::AnimatedGraphics
(
	std::string&& texture,
	std::vector<std::pair<int, int>> frames,
	bool flip_x,
	bool flip_y,
	double rotation,
	bool priority,
	int x_adjustment,
	int y_adjustment,
	int w_adjustment,
	int h_adjustment,
	int animation_speed
)
:
	SpriteGraphics( std::forward<std::string> ( texture ), 0, 0, flip_x, flip_y, rotation, priority, x_adjustment, y_adjustment, w_adjustment, h_adjustment, animation_speed ),
	frames_ ( frames ),
	current_frame_index_ ( 0, frames.size() - 1, 0, true )
{
	setFrames();
};

AnimatedGraphics::~AnimatedGraphics() {};

void AnimatedGraphics::update()
{
	if ( animation_timer_.hit() )
	{
		++current_frame_index_;
		setFrames();
	}
	animation_timer_.update();
};

int AnimatedGraphics::getXFrameOfPairIndex( int index ) const
{
	return std::get<X_INDEX>( frames_.at( index ) );
};

int AnimatedGraphics::getYFrameOfPairIndex( int index ) const
{
	return std::get<Y_INDEX>( frames_.at( index ) );
};

void AnimatedGraphics::setFrames()
{
	if ( current_frame_index_ < frames_.size() )
	{
		current_frame_x_ = getXFrameOfPairIndex( current_frame_index_() );
		current_frame_y_ = getYFrameOfPairIndex( current_frame_index_() );
	}
};