#ifndef GOAL_H
#define GOAL_H

class EventSystem;
class Input;
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

		virtual void update( SpriteSystem& sprites, InventoryLevel& inventory, const Map& lvmap, EventSystem& events, const Input& input );
		void init( Sprite& hero, InventoryLevel& inventory, EventSystem& events );
		virtual void customInit( Sprite& hero, InventoryLevel& inventory, EventSystem& events );

		std::string message();

	private:
		const std::string message_;
};

#endif // GOAL_H