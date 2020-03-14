#pragma once

#include "map_layer_image.hpp"

class MapLayerImageSwitch final : public MapLayerImage
{
	public:
		static constexpr int REPEAT_INFINITE = 255;
		MapLayerImageSwitch
		(
			std::string&& texture,
			int width,
			int height,
			int offset_x = 0,
			int offset_y = 0,
			double scroll_speed_x = 1,
			double scroll_speed_y = 1,
			int num_o_frames = 1,
			int repeat_x = REPEAT_INFINITE,
			int repeat_y = REPEAT_INFINITE,
			int move_speed_x = 0,
			int move_speed_y = 0,
			int animation_speed = 1,
			bool flip = false,
			Uint8 alpha = 255,
			SDL_BlendMode blend_mode = SDL_BLENDMODE_NONE
		);
		~MapLayerImageSwitch();
		void update( LevelState& level_state ) override;
};
