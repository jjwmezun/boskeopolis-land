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

		SpriteSystem( LevelState& level_state );
		~SpriteSystem();
		SpriteSystem( const SpriteSystem& ) = delete;
		SpriteSystem& operator= ( const SpriteSystem& ) = delete;

		void update( LevelState& level_state );
		void renderHero( const LevelState& level_state ) const;
		void renderSprite( int id, const LevelState& level_state ) const;
		void interact( LevelState& level_state );
		void spriteInteraction( LevelState& level_state );
		void reset( LevelState& level_state );
		void resetTrainer( LevelState& level_state );
		void spawn( std::unique_ptr<Sprite>&& sprite );
		void spawnEnemyBullet( int x, int y, Direction::Simple direction );
		void spawnHeroBullet( int x, int y, Direction::Simple direction );
		void heroOpenTreasureChest();
		Sprite& hero();
		const Sprite& hero() const;
		void interactWithMap( LevelState& level_state );
		int permanentlyKilledEnemies() const;
		int numberOfSprites() const;
		bool spritesOnScreen( const Camera& camera, const Sprite::SpriteType type ) const;
		const std::vector<std::unique_ptr<Sprite>>& getSpritesList() const;
		void changeHeroToShooter();


	private:
		void clearSprites();
		void destroySprite( int n, Map& lvmap );
		void spritesFromMap( LevelState& level_state );
		void addSprite( std::unique_ptr<Sprite>&& sprite );
		std::unique_ptr<Sprite> spriteType( int type, int x, int y, int i, LevelState& level_state );
		void resetInternal( LevelState& level_state, bool trainer );

		int id_;
		int permanently_killed_enemies_;
		LevelState& level_state_;
		std::unique_ptr<Sprite> hero_;
		std::vector<std::unique_ptr<Sprite>> sprites_;
		std::unordered_map<int, Sprite*> sprites_map_;
};
