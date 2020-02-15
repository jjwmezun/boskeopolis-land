#pragma once

#include "block_system.hpp"
#include "camera.hpp"
#include "game_state.hpp"
#include "event_system.hpp"
#include "health.hpp"
#include "inventory_level.hpp"
#include "level.hpp"
#include "level_graphics_system.hpp"
#include "sprite.hpp"
#include "player_graphics.hpp"
#include "sprite_system.hpp"

class LevelState : public GameState
{
	public:
		LevelState( int level_id, Camera camera = { Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS - 32, 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS} );
		~LevelState();
		void stateUpdate() override;
		void stateRender() override;
		void init() override;
		void backFromPop() override;
		void renderLevel();
		void updateForTrainer();
		void initForTrainer();

		Map& currentMap();

	private:
		void testPause();

		Health health_;
		Camera camera_;
		LevelGraphicsSystem graphics_;
		SpriteSystem sprites_;
		BlockSystem blocks_;
		EventSystem events_;
		Level level_;
		InventoryLevel inventory_screen_;
};
