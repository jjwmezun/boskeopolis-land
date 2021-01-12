#pragma once

class BlockSystem;
class Camera;
class Health;
class InventoryLevel;
class LevelState;
class Sprite;
class SpriteSystem;

#include "goal.hpp"
#include "map.hpp"
#include "rapidjson/document.h"
#include <string>
#include <vector>

class Level final
{
	public:
		friend class LevelList;

		static constexpr int NUMBER_OF_THEMES = 16;
		static constexpr int NUMBER_OF_CYCLES = 4;
		static constexpr int NUMBER_OF_LEVELS = NUMBER_OF_THEMES * NUMBER_OF_CYCLES + 2;

		~Level();
		Level( Level&& m );
		Level( const Level& ) = delete;
		Level&& operator=( Level&& ) = delete;
		const Level& operator=( const Level& ) = delete;

		void init( LevelState& level_state );
		void initCurrentMap( LevelState& level_state );
		void updateGoal( LevelState& level_state );
		Map& currentMap();
		const Map& currentMap() const;
		int entranceX() const;
		int entranceY() const;
		int cameraX() const;
		int cameraY() const;
		void warp( LevelState& level_state );
		void sewerWarp( SpriteSystem& sprites, EventSystem& events );
		const std::u32string& message() const;
		const std::string& getCodeName() const;
		int allEnemiesToKill() const;
		bool startOn() const;
		int id() const;

		static void buildCodeNames();
		static void buildLevelList();

	private:
		Level
		(
			int id,
			const std::vector<Map>& maps,
			std::unique_ptr<Goal> goal,
			int entrance_x = 0,
			int entrance_y = 0,
			int camera_x = 0,
			int camera_y = 0,
			std::u32string message = U"",
			bool start_on = false
		);

		const bool start_on_;
		const int id_;
		const int camera_x_;
		const int camera_y_;
		int entrance_x_;
		int entrance_y_;
		unsigned int current_map_;
		std::unique_ptr<Goal> goal_;
		std::vector<Map> maps_;
		const std::u32string message_;
		unsigned int testCurrentMap() const;
};
