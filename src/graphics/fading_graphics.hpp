#pragma once

#include "sprite_graphics.hpp"

class FadingGraphics final : public SpriteGraphics
{
	public:
		FadingGraphics
		(
			std::string&& texture,
			int frame_x = 0,
			int frame_y = 0,
			bool start_on = true,
			int speed = 8,
			bool flip_x = false,
			bool flip_y = false,
			double rotation = 0,
			int x_adjustment = 0,
			int y_adjustment = 0,
			int w_adjustment = 0,
			int h_adjustment = 0,
			int animation_speed = 8
		);
		~FadingGraphics();
		void update() override;

	private:
		const int change_per_update_;
		bool fading_in_;
};