#ifndef SPRITE_SYSTEM_H
#define SPRITE_SYSTEM_H

class BlockSystem;
class Camera;
class EventSystem;
class Game;
class Graphics;
class Input;
class InventoryLevel;
class MapLayer;

#include <memory>
#include "sprite.h"
#include "player_sprite.h"
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
			FLUTTERING
		};

		static HeroType heroType( std::string property );

		enum class SpawnSprite
		{
			SPIKE_EGG
		};

		SpriteSystem( int entrance_x, int entrance_y );
		~SpriteSystem();
		SpriteSystem( const SpriteSystem& ) = delete;
		SpriteSystem& operator= ( const SpriteSystem& ) = delete;

		void update( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, BlockSystem& blocks );
		void render( Graphics& graphics, Camera& camera, bool priority = false );
		void interact( BlockSystem& blocks, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );
		void spriteInteraction( Camera& camera, BlockSystem& blocks, Map& lvmap );
		void reset( const Level& level, const InventoryLevel& inventory );
		void spawn( SpawnSprite type, int x, int y );
		void spawnCactooieSpine( int x, int y, Direction::Horizontal direction );
		Sprite& hero();
		void interactWithMap( Map& lvmap, Camera& camera );


	private:
		static constexpr int SPRITE_INDEX_START = 400;
		static constexpr int OFFSCREEN_PADDING = Unit::BlocksToPixels( 2 );

		std::vector<std::unique_ptr<Sprite>> sprites_;
		std::unique_ptr<Sprite> hero_;

		void clearSprites();
		void destroySprite( int n );
		void spritesFromMap( const Map& lvmap );
		std::unique_ptr<Sprite> spriteType( int type, int x, int y, const Map& lvmap );
		
		void testNumOSprites() const;
};

#endif // SPRITE_SYSTEM_H