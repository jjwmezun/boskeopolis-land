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
#include <vector>

class Level
{
	public:
		static constexpr int MAX = 64;
		static Level getLevel( int id );

		~Level();
		Level( Level&& m );
		Level( const Level& ) = delete;
		Level&& operator=( Level&& ) = delete;
		const Level& operator=( const Level& ) = delete;

		Map& currentMap();
		const Map& currentMap() const;

		int entranceX() const;
		int entranceY() const;
		int cameraX() const;
		int cameraY() const;
		void warp( SpriteSystem& sprites, Camera& camera, EventSystem& events, BlockSystem& blocks );
		void sewerWarp( SpriteSystem& sprites, EventSystem& events );
		const std::string& message() const;
		int id() const;
		int allEnemiesToKill() const;

		void init( Sprite& hero, InventoryLevel& inventory, EventSystem& events, Health& health );
		void updateGoal( InventoryLevel& inventory_screen, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, const Camera& camera, Health& health, LevelState& state );

		static const std::string& NameOLevel( unsigned int n );
		static unsigned int realLevelNum();
		static int gemChallenge( unsigned int n );
		static int timeChallenge( unsigned int n );
		static std::string gemChallengeText( unsigned int n );
		static std::string timeChallengeText( unsigned int n );

	private:
		std::vector<Map> maps_;
		const std::string message_;
		std::unique_ptr<Goal> goal_;
		unsigned int current_map_;
		const int id_;
		const int camera_x_;
		const int camera_y_;
		int entrance_x_;
		int entrance_y_;

		Level
		(
			int id,
			const std::vector<Map>& maps,
			std::unique_ptr<Goal> goal,
			int entrance_x = 0,
			int entrance_y = 0,
			int camera_x = 0,
			int camera_y = 0,
			std::string message = ""
		);

		unsigned int testCurrentMap() const;

		static void buildLevelList();
		static void checkLvList();
};
