#pragma once

#include "block_system.hpp"
#include "camera.hpp"
#include "game_state.hpp"
#include "event_system.hpp"
#include "health.hpp"
#include "inventory_level.hpp"
#include "level.hpp"
#include "sprite.hpp"
#include "option_system.hpp"
#include "player_graphics.hpp"
#include "sprite_system.hpp"

class LevelTitleState : public GameState
{
	public:
		LevelTitleState();
		~LevelTitleState();
        LevelTitleState( const LevelTitleState& ) = delete;
        LevelTitleState& operator=( const LevelTitleState& ) = delete;
        LevelTitleState( LevelTitleState&& ) = delete;
        LevelTitleState& operator=( LevelTitleState&& ) = delete;
		void stateUpdate() override;
		void stateRender() override;
		void init() override;
		void backFromPop() override;

	private:
		InventoryLevel inventory_screen_;
		Level level_;
		EventSystem events_;
		Camera camera_;
		SpriteSystem sprites_;
		BlockSystem blocks_;
		Health health_;

		OptionSystem options_;
        SDL_Texture* screen_texture_;
        sdl2::SDLRect screen_src_;
        sdl2::SDLRect screen_dest_;
        sdl2::SDLRect curtain_rect_;
		SpriteGraphics logo_gfx_;
		const sdl2::SDLRect logo_rect_;

		bool can_load_;
        bool paused_;

		void testPause();
};
