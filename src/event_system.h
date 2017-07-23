#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include "palette.h"

class BlockSystem;
class Camera;
class InventoryLevel;
class Level;
class SpriteSystem;

class EventSystem
{
	public:
		EventSystem();

		void update( Level& level, InventoryLevel& inventory, SpriteSystem& sprites, Camera& camera, BlockSystem& blocks );
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

		int move_water_;
		int current_water_;
		bool waterShouldMove() const;
		bool waterShouldStop() const;
		bool in_front_of_door_;


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
		
		void testMessage( Level& level );
		void testWarp( Level& level, InventoryLevel& inventory, SpriteSystem& sprites, Camera& camera, BlockSystem& blocks );
		void testWinLoseOrQuit( Level& level, InventoryLevel& inventory );
		void failEvent( Level& level, InventoryLevel& inventory );
		void winEvent( Level& level, InventoryLevel& inventory );
		void quitEvent( Level& level, InventoryLevel& inventory );
};

#endif // EVENT_SYSTEM_H
