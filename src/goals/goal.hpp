#pragma once

class EventSystem;
class Health;
class InventoryLevel;
class Level;
class LevelState;
class Map;
class Sprite;
class SpriteSystem;

#include <string>

class Goal
{
	public:
		Goal( std::u32string message );
		virtual ~Goal();
		virtual void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state );
		void init( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health );
		virtual void customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health );

	protected:
		static std::u32string formatMessage( std::u32string message, int amount_needed );

	private:
		const std::u32string message_;
};