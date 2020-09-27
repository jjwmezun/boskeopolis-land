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
		static constexpr int REPEAT_INFINITE = 255;
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
			Uint8 alpha = 255,
			SDL_BlendMode blend_mode = SDL_BLENDMODE_NONE,
			int layer_position = DEFAULT_POSITION
		);
		~MapLayerImage();
		virtual void update( LevelState& level_state ) override;
		void render( const Camera& camera ) override;
		void move( int width, const sdl2::SDLRect& container );
		void render( const sdl2::SDLRect& container );

	protected:
		TimerRepeat animation_timer_;
		Counter current_frame_;
		const std::string texture_;
		sdl2::SDLRect source_;
		const double scroll_speed_x_;
		const double scroll_speed_y_;
		const int offset_x_;
		const int offset_y_;
		const int repeat_x_;
		const int repeat_y_;
		const int move_speed_x_;
		const int move_speed_y_;
		const int num_o_frames_;
		const int animation_speed_;
		const SDL_BlendMode blend_mode_;
		int_fast32_t movement_position_x_;
		int_fast32_t movement_position_y_;
		Direction::Vertical frame_dir_;
		Uint8 alpha_;

		void renderY( const sdl2::SDLRect& container ) const;
		void renderX( const sdl2::SDLRect& container, sdl2::SDLRect& dest ) const;
		void renderFinal( sdl2::SDLRect& dest ) const;

		static constexpr bool onscreen( const sdl2::SDLRect& r, const sdl2::SDLRect& container )
		{
			return
				r.right() > 0 &&
				r.x < container.w &&
				r.bottom() > 0 &&
				r.y < container.h;
		};
};
