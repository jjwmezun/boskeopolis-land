#pragma once

#include "counter.hpp"
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
			bool repeat_x = true,
			bool repeat_y = true,
			int move_speed_x = 0,
			int move_speed_y = 0,
			int animation_speed = 1,
			bool flip = false,
			Uint8 alpha = 255
		);
		void update( EventSystem& events, BlockSystem& blocks, const Camera& camera ) override;
		void render( Camera& camera ) const override;

	private:
		const std::string texture_;
		sdl2::SDLRect source_;
		const int offset_x_;
		const int offset_y_;
		const double scroll_speed_x_;
		const double scroll_speed_y_;
		const bool repeat_x_;
		const bool repeat_y_;
		const int move_speed_x_;
		const int move_speed_y_;
		const int num_o_frames_;
		const int animation_speed_;
		const Uint8 alpha_;
		int movement_position_x_;
		int movement_position_y_;
		Counter current_frame_;
		TimerRepeat animation_timer_;
		Direction::Vertical frame_dir_;

		void renderY( Camera& camera ) const;
		void renderX( Camera& camera, sdl2::SDLRect& dest ) const;
		bool onscreen( const sdl2::SDLRect& r, Camera& camera ) const;
};