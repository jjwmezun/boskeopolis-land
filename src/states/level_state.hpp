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

class LevelState final : public GameState
{
	public:
		LevelState( int level_id, Difficulty difficulty, int heart_upgrades, bool has_oxygen_upgrade, Camera camera = { Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS - 32, 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS} );
		~LevelState();
		void stateUpdate() override;
		void stateRender() override;
		void init() override;
		void backFromPop() override;
		void renderLevel();
		void updateForTrainer();
		void initForTrainer();

		Health& health();
		const Health& health() const;
		Camera& camera();
		const Camera& camera() const;
		SpriteSystem& sprites();
		const SpriteSystem& sprites() const;
		BlockSystem& blocks();
		const BlockSystem& blocks() const;
		EventSystem& events();
		const EventSystem& events() const;
		Level& level();
		const Level& level() const;
		Map& currentMap();
		const Map& currentMap() const;
		InventoryLevel& inventory();
		const InventoryLevel& inventory() const;

	private:
		void testPause();

		Health health_;
		Camera camera_;
		SpriteSystem sprites_;
		BlockSystem blocks_;
		EventSystem events_;
		Level level_;
		InventoryLevel inventory_screen_;
};
