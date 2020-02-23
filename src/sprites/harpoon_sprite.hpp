#pragma once

#include "sprite.hpp"
#include "sprite_graphics.hpp"

class HarpoonSprite : public Sprite
{
	public:
		HarpoonSprite( int x, int y );
		~HarpoonSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( Camera& camera, bool priority );

	private:
		enum class HarpoonState
		{
			WAIT_TO_GROW,
			GROW,
			WAIT_TO_SHRINK,
			SHRINK
		};
		static constexpr int NUMBER_OF_FRAMES = 12;

		int delay_timer_;
		int grow_speed_;
		HarpoonState state_;
		sdl2::SDLRect left_harpoon_;
		sdl2::SDLRect right_harpoon_;
		sdl2::SDLRect left_harpoon_original_;
		sdl2::SDLRect right_harpoon_original_;
		SpriteGraphics left_harpoon_gfx_;
		SpriteGraphics right_harpoon_gfx_;
		int frames_[ NUMBER_OF_FRAMES ];
};
