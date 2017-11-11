#pragma once

#include "palette.hpp"

class BlockSystem;
class Camera;
class Level;
class SpriteSystem;

class EventSystem
{
	public:
	
	enum class EType
	{
		__NULL,
		LOCKED_GATE_OPENING,
		LOCKED_GATE_OPEN
	};

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
	bool palette_changed_;
	bool in_front_of_door_;
	bool on_conveyor_belt_;
	int move_water_;
	int current_water_;
	EType special_;
	Palette new_palette_;

	bool hasKey() const;
	bool paletteChanged() const;
	bool switchOn() const;
	bool switchChanged() const;
	void flipSwitch();
	bool waterShouldMove() const;
	bool waterShouldStop() const;

	EventSystem();
	void update( Level& level, SpriteSystem& sprites, Camera& camera, BlockSystem& blocks );
	void reset();
	Palette getPalette();
	bool testLightSwitch();
	void win();
	void fail();
	void quitLevel();
	void changeMap();
	void getKey();
	void showMessage();
	void changePalette( const Palette& new_palette );
	void lightSwitchTurnOn();
	void resetPalette();
	void testMessage( Level& level );
	void testWarp( Level& level, SpriteSystem& sprites, Camera& camera, BlockSystem& blocks );
	void testWinLoseOrQuit( Level& level );
	void failEvent( Level& level );
	void winEvent( Level& level );
	void quitEvent( Level& level );
};