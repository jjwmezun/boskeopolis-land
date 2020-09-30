#pragma once

#include "counter.hpp"
#include "sprite_graphics.hpp"
#include "timer_repeat.hpp"
#include <vector>
#include "unit.hpp"
#include <utility>

class AnimatedGraphics final : public SpriteGraphics
{
	public:
		AnimatedGraphics
		(
			std::string&& texture,
			std::vector<std::pair<int, int>> frames,
			bool flip_x = false,
			bool flip_y = false,
			double rotation = 0,
			int x_adjustment = 0,
			int y_adjustment = 0,
			int w_adjustment = 0,
			int h_adjustment = 0,
			int animation_speed = Unit::DEFAULT_ANIMATION_SPEED,
			Uint8 alpha = 255,
			SDL_BlendMode blend_mode = SDL_BLENDMODE_NONE
		);
		~AnimatedGraphics();
		void update() override;

	private:
		std::vector<std::pair<int, int>> frames_;
		Counter current_frame_index_;
		TimerRepeat animation_timer_;

		int getXFrameOfPairIndex( int index ) const;
		int getYFrameOfPairIndex( int index ) const;
		void setFrames();
};