#ifndef LEVEL_STATE_H
#define LEVEL_STATE_H

#include "block_system.h"
#include "camera.h"
#include "game_state.h"
#include "event_system.h"
#include "inventory_level.h"
#include "level.h"
#include "sprite.h"
#include "player_graphics.h"
#include "sprite_system.h"

class LevelState : public GameState
{

	public:
		LevelState( const EventSystem& events, const Inventory& inventory, int lvname );
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

#endif // LEVEL_STATE_H
