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
			CAR
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
		void reset( const Level& level );
		void spawn( SpawnSprite type, int x, int y );
		void spawnCactooieSpine( int x, int y, Direction::Horizontal direction );
		void spawnSnowball( int x, int y, Direction::Horizontal direction );
		void spawnWaterdrop( int x, int y );
		void spawnEnemyBullet( int x, int y, Direction::Simple direction );
		void spawnHeroBullet( int x, int y, Direction::Simple direction, SpriteGraphics* gfx = nullptr );
		Sprite& hero();
		const Sprite& hero() const;
		void interactWithMap( Map& lvmap, Camera& camera, Health& health );
		int permanentlyKilledEnemies() const;


	private:
		static constexpr int SPRITE_INDEX_START = 400;
		static constexpr int OFFSCREEN_PADDING = Unit::BlocksToPixels( 2 );

		std::vector<std::unique_ptr<Sprite>> sprites_;
		std::unique_ptr<Sprite> hero_;
		int permanently_killed_enemies_;

		void clearSprites();
		void destroySprite( int n, Map& lvmap );
		void spritesFromMap( const Map& lvmap );
		std::unique_ptr<Sprite> spriteType( int type, int x, int y, int i, const Map& lvmap );
		
		void testNumOSprites() const;
};