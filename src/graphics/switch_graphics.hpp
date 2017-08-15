#pragma once

#include "counter.hpp"
#include "sprite_graphics.hpp"
#include <vector>
#include <utility>

class SwitchGraphics : public SpriteGraphics
{
	public:
		SwitchGraphics
		(
			std::string&& texture,
			std::pair<int, int> frames_on,
			std::pair<int, int> frames_off,
			bool flip_x = false,
			bool flip_y = false,
			double rotation = 0,
			bool priority = false,
			int x_adjustment = 0,
			int y_adjustment = 0,
			int w_adjustment = 0,
			int h_adjustment = 0
		);
		~SwitchGraphics();
		void update();

	private:
		static const int X_INDEX = 0;
		static const int Y_INDEX = 1;

		std::pair<int, int> frames_on_;
		std::pair<int, int> frames_off_;

		void setOn();
		void setOff();
};