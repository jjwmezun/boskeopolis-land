#pragma once

#include "block_system.hpp"
#include "camera.hpp"
#include "game_state.hpp"
#include "event_system.hpp"
#include "health.hpp"
#include "inventory_level.hpp"
#include "level.hpp"
#include "sprite.hpp"
#include "player_graphics.hpp"
#include "sprite_system.hpp"

class LevelState : public GameState
{
	public:
		LevelState( int level_id );
		~LevelState();
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

		void testPause();
};
