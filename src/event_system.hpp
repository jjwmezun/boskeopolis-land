#pragma once

#include "palette.hpp"
#include <SDL2/SDL.h>

class Camera;
class Level;
class LevelState;
class InventoryLevel;
class SpriteSystem;
class TextObj;

class EventSystem final
{
	public:
		EventSystem();
		~EventSystem();
		void init( const Level& level );
		void update( LevelState& level_state );
		void updateTrainer( LevelState& level_state );
		void reset();
		bool hasKey() const;
		bool paletteChanged() const;
		bool isSwitchOn() const;
		bool switchChanged() const;
		void flipSwitch();
		bool waterShouldMove() const;
		bool waterShouldStop() const;
		Palette getPalette();
		bool testLightSwitch();
		void win();
		void fail();
		void secretGoal();
		void quitLevel();
		void changeMap();
		void changeMapSewer();
		void getKey();
		void showMessage();
		void changePalette( const Palette& new_palette );
		void resetPalette();
		void testMessage( Level& level );
		void testWarp( LevelState& level_state );
		void playDeathSoundIfNotAlreadyPlaying();
		void startTimer();
		bool timerStart() const;
		void unsetFlag();
		bool trainDoorPartlyOpen() const;
		void setOpening();
		void setOpen();
		int addRandomTreasure();
		void setRandomTreasureIfNotAlreadySet();
		bool testRandomTreasure( int id );
		void setSewerPosition( int x, int y );
		bool onUpperLayer() const;
		bool showBossUI() const;
		void renderBossUI() const;
		void createBossUI();
		void changeBossUI( const TextObj& text, int health );
		int waterMovement() const;
		void setWaterMovement( int value );
		bool isWaterMovingUpward() const;
		bool isWaterMovingDownward() const;
		void setCurrentWater( int value );
		bool pauseStateMovement() const;
		bool pauseIsDisabled() const;
		void setPauseDisabled();
		bool testTouchingLadder() const;
		void setTouchingLadder();
		void resetTouchingLadder();
		void setInFrontOfDoor();
		void setInFrontOfSewerDoor();
		void resetInFrontOfDoor();
		bool isInFrontOfRegularDoor() const;
		bool isInFrontOfSewerDoor() const;
		void forceSwitch( bool value );
		void forceSwitchOn();
		void forceSwitchOff();
		void setCanClimbDown();
		void resetCanClimbDown();
		bool testCanClimbDown() const;
		void setOnConveyorBelt();
		void resetOnConveyorBelt();
		bool testOnConveyorBelt() const;
		bool testIsSliding() const;
		void setSlidingOn();
		void resetIsSliding();
		bool testIsSlidingPreviously() const;
		bool testChangeMap() const;
		bool testPauseHero() const;
		void setPauseHeroOn();
		void setPauseHeroOff();

	private:
		enum class LevelCompleteStatus
		{
			STILL_PLAYING,
			WON,
			FAILED,
			QUIT,
			SECRET_GOAL
		};

		enum class MiscFlagType
		{
			__NULL,
			LOCKED_GATE_OPENING,
			LOCKED_GATE_OPEN,
			TIMER_START,
			UPPER_LAYER
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
			FLAG,
			BOSS_UI
		};

		struct SewerGFX
		{
			SewerGFX( int x, int y );
			sdl2::SDLRect dest_;
			bool fade_in_;
		};

		struct BossUI
		{
			BossUI();
			~BossUI();
			BossUI( const BossUI& ) = delete;
			BossUI& operator=( const BossUI& ) = delete;
			BossUI( BossUI&& ) = delete;
			BossUI& operator=( BossUI&& ) = delete;
			void render() const;

			sdl2::SDLRect dest_;
			sdl2::SDLRect src_;
			SDL_Texture* texture_;
		};

		union MiscData
		{
			MiscData();
			RandTreasure rand_treasure_;
			SewerGFX* sewer_gfx_;
			MiscFlagType flag_;
			BossUI* boss_ui_;
		};

		struct EMisc
		{
			EMisc();
			MiscType type_;
			MiscData data_;
		};

		enum class Door
		{
			NONE,
			REGULAR,
			SEWER
		};

		void doWarp( LevelState& level_state );
		void testLevelCompleteStatus( const InventoryLevel& inventory );
		void failEvent();
		void winEvent( const InventoryLevel& inventory );
		void quitEvent();
		void secretGoalEvent( const InventoryLevel& inventory );
		bool testFlag( MiscFlagType type ) const;
		void setFlag( MiscFlagType type );
		void assertFlag() const;
		bool testIsFlag() const;
		void resetMisc();
		bool testSewerAnimationFinished() const;
		void turnOnSewerAnimation();
		void doSewerAnimation( SpriteSystem& sprites );
		void renderSewer( const Camera& camera );
		bool ensureSewerPointerIsSet( int x, int y );

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
		bool touching_ladder_;
		bool touching_ladder_prev_;
		bool can_climb_down_;
		bool is_sliding_;
		bool is_sliding_prev_;
		bool pause_hero_;
		int change_map_;
		LevelCompleteStatus level_complete_status_;
		Door in_front_of_door_;
		int move_water_;
		int current_water_;
		Palette new_palette_;
		EMisc misc_;
};
