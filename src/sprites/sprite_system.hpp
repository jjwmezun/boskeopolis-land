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

class SpriteSystem
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
			PLANE
		};

		static HeroType heroType( const std::string& property );

		enum class SpawnSprite
		{
			SPIKE_EGG
		};

		SpriteSystem( int entrance_x, int entrance_y );
		~SpriteSystem();
		SpriteSystem( const SpriteSystem& ) = delete;
		SpriteSystem& operator= ( const SpriteSystem& ) = delete;

		void update( Camera& camera, Map& lvmap, EventSystem& events, BlockSystem& blocks, Health& health );
		void render( Camera& camera, bool priority = false );
		void interact( BlockSystem& blocks, Level& level, EventSystem& events, Camera& camera, Health& health );
		void spriteInteraction( Camera& camera, BlockSystem& blocks, Map& lvmap, Health& health, EventSystem& events );
		void reset( const Level& level, EventSystem& events );
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
		void interactWithMap( Map& lvmap, Camera& camera, Health& health );
		int permanentlyKilledEnemies() const;
		int numberOfSprites() const;
		bool spritesOnScreen( const Camera& camera, const Sprite::SpriteType type ) const;
		const std::vector<std::unique_ptr<Sprite>>& getSpritesList() const;


	private:
		static constexpr int SPRITE_INDEX_START = 400;
		static constexpr int OFFSCREEN_PADDING = Unit::BlocksToPixels( 2 );

		std::vector<std::unique_ptr<Sprite>> sprites_;
		std::unique_ptr<Sprite> hero_;
		int permanently_killed_enemies_;

		void clearSprites();
		void destroySprite( int n, Map& lvmap );
		void spritesFromMap( const Map& lvmap, EventSystem& events );
		std::unique_ptr<Sprite> spriteType( int type, int x, int y, int i, const Map& lvmap, EventSystem& events );
};
