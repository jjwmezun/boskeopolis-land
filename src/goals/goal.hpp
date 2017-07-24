#ifndef GOAL_H
#define GOAL_H

class EventSystem;

class InventoryLevel;
class Map;
class Sprite;
class SpriteSystem;

#include <string>

class Goal
{

	public:
		Goal( std::string message = "¡Get the Keycane!" );
		virtual ~Goal();

		virtual void update( SpriteSystem& sprites, const Map& lvmap, EventSystem& events );
		void init( Sprite& hero, InventoryLevel& inventory_screen, EventSystem& events );
		virtual void customInit( Sprite& hero, InventoryLevel& inventory_screen, EventSystem& events );

		std::string message();

	private:
		const std::string message_;
};

#endif // GOAL_H