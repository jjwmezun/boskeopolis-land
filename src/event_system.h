#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include "palette.h"

class Camera;
class Game;
class InventoryLevel;
class Level;
class SpriteSystem;

class EventSystem
{
	public:
		EventSystem();

		void update( Level& level, InventoryLevel& inventory, Game& game, SpriteSystem& sprites, Camera& camera );
		void reset();
		
		void win();
		void fail();
		void quitLevel();
		void changeMap();
		
		void getKey();
		bool hasKey() const;
		
		void showMessage();
		
		void changePalette( const Palette& new_palette );
		Palette getPalette();	
		bool paletteChanged() const;

		bool switchOn() const;
		bool switchChanged() const;
		void flipSwitch();

		int move_water_ = -1;
		int current_water_ = -1;
		bool waterShouldMove() const;
		bool waterShouldStop() const;


   private:
		static constexpr int WATER_NULL = -1;

		bool won_;
		bool failed_;
		bool quit_level_;
		bool key_;
		bool message_;
		bool message_lock_;
		bool change_map_;
		bool switch_;
		bool switch_changed_;
		Palette new_palette_;
		bool palette_changed_;

		void resetPalette();
		
		void testMessage( Level& level, Game& game );
		void testWarp( Level& level, InventoryLevel& inventory, SpriteSystem& sprites, Camera& camera );
		void testWinLoseOrQuit( Level& level, InventoryLevel& inventory, Game& game );
		void failEvent( Level& level, InventoryLevel& inventory, Game& game );
		void winEvent( Level& level, InventoryLevel& inventory, Game& game );
		void quitEvent( Level& level, InventoryLevel& inventory, Game& game );
};

#endif // EVENT_SYSTEM_H
