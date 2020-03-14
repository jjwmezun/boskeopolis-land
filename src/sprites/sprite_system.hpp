#pragma once

class BlockSystem;
class Camera;
class EventSystem;
class Health;
class MapLayer;

#include <memory>
#include "sprite.hpp"
#include "player_sprite.hpp"
#include <unordered_map>
#include <vector>

class LevelState;

class SpriteSystem final
{
	public:
		enum class HeroType
		{
			NORMAL,
			CART,
			OVERWORLD,
			FLUTTERING,
			SHOOTER,
			SPACESHIP,
			CAR,
			TOP_DOWN,
			DOOM,
			PLANE,
			FLASHLIGHT
		};

		static HeroType heroType( const std::string& property );

		enum class SpawnSprite
		{
			SPIKE_EGG
		};

		SpriteSystem();
		~SpriteSystem();
		SpriteSystem( const SpriteSystem& ) = delete;
		SpriteSystem& operator= ( const SpriteSystem& ) = delete;

		void update( LevelState& level_state );
		void render( Camera& camera, bool priority = false );
		void renderSuperPriority( Camera& camera );
		void interact( LevelState& level_state );
		void spriteInteraction( LevelState& level_state );
		void reset( LevelState& level_state );
		void resetTrainer( LevelState& level_state );
		void spawn( std::unique_ptr<Sprite>&& sprite );
		void spawnCactooieSpine( int x, int y, Direction::Horizontal direction );
		void spawnWaterdrop( int x, int y );
		void spawnEnemyBullet( int x, int y, Direction::Simple direction );
		void spawnHeroBullet( int x, int y, Direction::Simple direction );
		void spawnShmupBullet( int x, int y, double dy, double dx );
		void spawnOlive( int x, int y, Direction::Horizontal start_dir );
		void spawnIcicle( int x, int y );
		void heroOpenTreasureChest();
		Sprite& hero();
		const Sprite& hero() const;
		void interactWithMap( LevelState& level_state );
		int permanentlyKilledEnemies() const;
		int numberOfSprites() const;
		bool spritesOnScreen( const Camera& camera, const Sprite::SpriteType type ) const;
		const std::vector<std::unique_ptr<Sprite>>& getSpritesList() const;


	private:
		static constexpr int SPRITE_INDEX_START = 400;
		static constexpr int OFFSCREEN_PADDING = Unit::BlocksToPixels( 2 );

		void clearSprites();
		void destroySprite( int n, Map& lvmap );
		void spritesFromMap( LevelState& level_state );
		std::unique_ptr<Sprite> spriteType( int type, int x, int y, int i, LevelState& level_state );
		void resetInternal( LevelState& level_state, bool trainer );

		int permanently_killed_enemies_;
		std::unique_ptr<Sprite> hero_;
		std::vector<std::unique_ptr<Sprite>> sprites_;
};
