#pragma once

#include "block_system.hpp"
#include "camera.hpp"
#include "game_state.hpp"
#include "event_system.hpp"
#include "health.hpp"
#include "inventory_level.hpp"
#include "level.hpp"
#include <map>
#include <memory>
#include "sprite.hpp"
#include "palette_transition.hpp"
#include "player_graphics.hpp"
#include "renderable.hpp"
#include "sprite_system.hpp"
#include "unit.hpp"

class Palette;

class LevelState final : public GameState
{
	public:
		static constexpr int NUMBER_OF_LAYERS = Unit::NUMBER_OF_LAYERS;

		LevelState( int level_id, Difficulty difficulty, int heart_upgrades, bool has_oxygen_upgrade, Camera camera = { Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS - 32, 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS} );
		~LevelState();
		void stateUpdate() override;
		void stateRender() override;
		void init() override;
		void backFromPop() override;
		void renderLevel();
		void updateForTrainer();
		void initForTrainer();
		int addRenderable( std::unique_ptr<Renderable>&& renderable, Unit::Layer layer );
		void removeRenderable( int id );
		void changeRenderableLayer( int id, Unit::Layer layer );

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
		bool trainerPaletteChanged() const;
		Palette getNewPalette();
		void reRenderInventory();
		void changePaletteTransition( const PaletteTransition& palette_transition );

	private:
		void testPause();

		int id_;
		std::vector<std::unique_ptr<Renderable>> layers_[ NUMBER_OF_LAYERS ];
		std::map<int, Renderable*> renderable_map_;
		Health health_;
		Camera camera_;
		SpriteSystem sprites_;
		BlockSystem blocks_;
		EventSystem events_;
		Level level_;
		InventoryLevel inventory_screen_;
		PaletteTransition palette_transition_;
};
