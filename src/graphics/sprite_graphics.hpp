#pragma once

class Camera;
class Sprite;

#include "render.hpp"
#include "timers/timer_repeat.hpp"


class SpriteGraphics
{
	public:
		SpriteGraphics
		(
			std::string&& texture,
			int current_frame_x = 0,
			int current_frame_y = 0,
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

		bool flip_x_;
		bool flip_y_;
		double rotation_;
		bool priority_;
		int current_frame_x_;
		int current_frame_y_;
		int x_adjustment_;
		int y_adjustment_;
		int w_adjustment_;
		int h_adjustment_;

		virtual ~SpriteGraphics();
		virtual void update( Sprite& sprite );
		virtual void update();
		virtual void render( const sdl2::SDLRect& bound_box, const Camera* camera = nullptr, bool priority = false, Uint8 alpha = 255 ) const;
		sdl2::SDLRect adjustBoundBox( const sdl2::SDLRect& r ) const;
		bool visible_ = true;


	protected:
		static constexpr int X_INDEX = 0;
		static constexpr int Y_INDEX = 1;

		TimerRepeat animation_timer_;
		std::string texture_;
		int prev_frame_x_;
		int prev_frame_y_;

		int cutoffLeft  ( int object_x, int camera_x ) const;
		int cutoffTop   ( int object_y, int camera_y ) const;
		int cutoffRight ( int object_x, int camera_x, int object_w, int camera_w ) const;
		int cutoffBottom( int object_y, int camera_y, int object_h, int camera_h ) const;
		int cutoffLeft  ( sdl2::SDLRect& obj_r, const Camera& c ) const;
		int cutoffTop   ( sdl2::SDLRect& obj_r, const Camera& c ) const;
		int cutoffRight ( sdl2::SDLRect& obj_r, const Camera& c ) const;
		int cutoffBottom( sdl2::SDLRect& obj_r, const Camera& c ) const;
		void masterRender( const sdl2::SDLRect& bound_box, int current_frame_x, int current_frame_y, const Camera* camera, bool priority, Uint8 alpha = 255 ) const;
};