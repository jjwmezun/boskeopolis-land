#pragma once

#include "counter.hpp"
#include <cstdint>
#include "map_layer.hpp"
#include "mezun_sdl2.hpp"
#include <SDL2/SDL.h>
#include <string>
#include "timers/timer_repeat.hpp"

class MapLayerImage : public MapLayer
{
	public:
		MapLayerImage
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
			Uint8 alpha = 255
		);
		void update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap ) override;
		void render( const Camera& camera ) const override;

	private:
		static constexpr int REPEAT_INFINITE = 255;

		const Uint8 alpha_;
		const int offset_x_;
		const int offset_y_;
		const int repeat_x_;
		const int repeat_y_;
		const int move_speed_x_;
		const int move_speed_y_;
		const int num_o_frames_;
		const int animation_speed_;
		int_fast32_t movement_position_x_;
		int_fast32_t movement_position_y_;
		Direction::Vertical frame_dir_;
		const double scroll_speed_x_;
		const double scroll_speed_y_;
		Counter current_frame_;
		TimerRepeat animation_timer_;
		const std::string texture_;
		sdl::rect source_;

		void renderY( const Camera& camera ) const;
		void renderX( const Camera& camera, sdl::rect& dest ) const;
		bool onscreen( const sdl::rect& r, const Camera& camera ) const;
};