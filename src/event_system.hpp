#pragma once

#include "palette.hpp"

class BlockSystem;
class Camera;
class Level;
class Sprite;
class SpriteSystem;

class EventSystem
{
	public:

	enum class MiscFlagType
	{
		__NULL,
		LOCKED_GATE_OPENING,
		LOCKED_GATE_OPEN,
		TIMER_START
	};

	struct RandTreasure
	{
		int count_ = 0;
		int keycane_chest_ = 0;
	};

	enum class MiscType
	{
		__NULL,
		RAND_TREASURE,
		SEWER_GFX,
		FLAG
	};

	struct SewerGFX
	{
		SewerGFX( int x, int y );
		sdl2::SDLRect dest_;
		bool fade_in_;
	};

	union MiscData
	{
		MiscData();
		RandTreasure rand_treasure_;
		SewerGFX* sewer_gfx_;
		MiscFlagType flag_;
	};

	struct EMisc
	{
		EMisc();
		MiscType type_;
		MiscData data_;
	};

	bool won_;
	bool failed_;
	bool quit_level_;
	bool key_;
	bool message_;
	bool message_lock_;
	bool switch_;
	bool switch_changed_;
	bool palette_changed_;
	bool on_conveyor_belt_;
	bool played_death_song_;
	bool disable_pause_;
	bool pause_state_movement_;
	int change_map_;
	int in_front_of_door_;
	int move_water_;
	int current_water_;
	Palette new_palette_;

	bool hasKey() const;
	bool paletteChanged() const;
	bool switchOn() const;
	bool switchChanged() const;
	void flipSwitch();
	bool waterShouldMove() const;
	bool waterShouldStop() const;

	EventSystem();
	~EventSystem();
	void update( Level& level, SpriteSystem& sprites, Camera& camera, BlockSystem& blocks );
	void reset();
	Palette getPalette();
	bool testLightSwitch();
	void win();
	void fail();
	void quitLevel();
	void changeMap();
	void changeMapSewer();
	void getKey();
	void showMessage();
	void changePalette( const Palette& new_palette );
	void lightSwitchTurnOn();
	void resetPalette();
	void testMessage( Level& level );
	void testWarp( Level& level, SpriteSystem& sprites, Camera& camera, BlockSystem& blocks );
	void doWarp( Level& level, SpriteSystem& sprites, Camera& camera, BlockSystem& blocks );
	void testWinLoseOrQuit( Level& level );
	void failEvent( Level& level );
	void winEvent( Level& level );
	void quitEvent( Level& level );
	void playDeathSoundIfNotAlreadyPlaying();
	void resetMisc();
	void setFlag( MiscFlagType type );
	void assertFlag() const;
	bool testIsFlag() const;
	bool testFlag( MiscFlagType type ) const;
	void startTimer();
	bool timerStart() const;
	void unsetFlag();
	bool trainDoorPartlyOpen() const;
	void setOpening();
	void setOpen();
	int addRandomTreasure();
	void setRandomTreasureIfNotAlreadySet();
	bool testRandomTreasure( int id );
	bool testSewerAnimationFinished() const;
	void turnOnSewerAnimation();
	void doSewerAnimation( SpriteSystem& sprites );
	void renderSewer( const Camera& camera );
	bool ensureSewerPointerIsSet( int x, int y );
	void setSewerPosition( int x, int y );

	private:
		EMisc misc_;
};
