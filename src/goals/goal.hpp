#ifndef GOAL_H
#define GOAL_H

class EventSystem;
class Health;
class InventoryLevel;
class Level;
class Map;
class Sprite;
class SpriteSystem;

#include <string>

class Goal
{

	public:
		Goal( std::string message = "¡Get the Keycane!" );
		virtual ~Goal();

		virtual void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health );
		void init( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health );
		virtual void customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health );

		std::string message();

	private:
		const std::string message_;
};

#endif // GOAL_H