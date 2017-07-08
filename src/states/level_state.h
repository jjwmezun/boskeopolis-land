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
		LevelState( EventSystem events, InventoryLevel inventory, int lvname, Game& game );
		~LevelState();

		void update( Game& game, const Input& input, Graphics& graphics );
		void stateRender( Graphics& graphics );
		void init( Game& game, Graphics& graphics );

	private:
		InventoryLevel inventory_;
		EventSystem events_;
		Level level_;
		Camera camera_;
		SpriteSystem sprites_;
		BlockSystem blocks_;
		
		void testPause( Game& game, const Input& input );
};

#endif // LEVEL_STATE_H
