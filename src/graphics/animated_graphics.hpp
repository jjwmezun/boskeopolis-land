#pragma once

#include "counter.hpp"
#include "sprite_graphics.hpp"
#include "timer_repeat.hpp"
#include <vector>
#include <utility>

class AnimatedGraphics : public SpriteGraphics
{
	public:
		AnimatedGraphics
		(
			std::string&& texture,
			std::vector<std::pair<int, int>> frames,
			bool flip_x = false,
			bool flip_y = false,
			double rotation = 0,
			bool priority = false,
			int x_adjustment = 0,
			int y_adjustment = 0,
			int w_adjustment = 0,
			int h_adjustment = 0,
			int animation_speed = 8
		);
		~AnimatedGraphics();
		void update();

	private:
		std::vector<std::pair<int, int>> frames_;
		Counter current_frame_index_;
		TimerRepeat animation_timer_;

		int getXFrameOfPairIndex( int index ) const;
		int getYFrameOfPairIndex( int index ) const;
		void setFrames();
};