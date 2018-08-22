#pragma once

class Camera;
class EventSystem;
class Sprite;

#include "direction.hpp"
#include "render.hpp"

class SpriteGraphics
{
	public:
		static constexpr int X_INDEX = 0;
		static constexpr int Y_INDEX = 1;

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
			Uint8 alpha = 255,
			SDL_BlendMode blend_mode = SDL_BLENDMODE_NONE
		);

		const std::string texture_;
		double rotation_;
		int prev_frame_x_;
		int prev_frame_y_;
		int current_frame_x_;
		int current_frame_y_;
		int x_adjustment_;
		int y_adjustment_;
		int w_adjustment_;
		int h_adjustment_;
		SDL_BlendMode blend_mode_;
		Uint8 alpha_;
		bool flip_x_;
		bool flip_y_;
		bool priority_;
		bool visible_;

		virtual ~SpriteGraphics();
		virtual void update();
		virtual void update( Sprite& sprite );
		virtual void update( const EventSystem& events );
		virtual void render( const sdl2::SDLRect& bound_box, const Camera* camera = nullptr, bool priority = false, SDL_Texture* texture = nullptr ) const;
		void renderAnyPriority( const sdl2::SDLRect& bound_box, const Camera* camera = nullptr ) const;
		void renderAnyPriorityOverrideAlpha( const sdl2::SDLRect& bound_box, Uint8 alpha, const Camera* camera = nullptr ) const;
		sdl2::SDLRect adjustBoundBox( const sdl2::SDLRect& r ) const;
		void masterRender( const sdl2::SDLRect& bound_box, int current_frame_x, int current_frame_y, const Camera* camera, bool priority, Uint8 alpha = 255, SDL_Texture* texture = nullptr ) const;
		void rotate( Direction::Clockwise dir, int amount = 5 );
};
