#pragma once

#include "block_system.hpp"
#include "camera.hpp"
#include "game_state.hpp"
#include "event_system.hpp"
#include "inventory_level.hpp"
#include "level.hpp"
#include "sprite.hpp"
#include "player_graphics.hpp"
#include "sprite_system.hpp"

class LevelState : public GameState
{

	public:
		LevelState( const Inventory& inventory, int lvname );
		~LevelState();

		void update();
		void stateRender();
		void init();

	private:
		InventoryLevel inventory_;
		EventSystem events_;
		Level level_;
		Camera camera_;
		SpriteSystem sprites_;
		BlockSystem blocks_;
		
		void testPause();
};