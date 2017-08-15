#pragma once

class Camera;
class Sprite;

#include "render.hpp"

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
			Uint8 alpha = 255
		);

		bool flip_x_;
		bool flip_y_;
		bool priority_;
		bool visible_;
		Uint8 alpha_;
		int current_frame_x_;
		int current_frame_y_;
		int x_adjustment_;
		int y_adjustment_;
		int w_adjustment_;
		int h_adjustment_;
		double rotation_;

		virtual ~SpriteGraphics();
		virtual void update( Sprite& sprite );
		virtual void update();
		virtual void render( const sdl2::SDLRect& bound_box, const Camera* camera = nullptr, bool priority = false ) const;
		sdl2::SDLRect adjustBoundBox( const sdl2::SDLRect& r ) const;


	protected:
		static constexpr int X_INDEX = 0;
		static constexpr int Y_INDEX = 1;

		std::string texture_;
		int prev_frame_x_;
		int prev_frame_y_;

		void masterRender( const sdl2::SDLRect& bound_box, int current_frame_x, int current_frame_y, const Camera* camera, bool priority, Uint8 alpha = 255 ) const;
};